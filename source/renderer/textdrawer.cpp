#include "textdrawer.hpp"

TextDrawer::TextDrawer(const char *fontPath, unsigned int nfontSize)
	: sx(1.f/800), sy(1.f/600), lineSpacing(1.35f)
{
	int err = FT_Init_FreeType(&ftLib);
	assertf(err == 0, "Failed to initialize FreeType");

	err = FT_New_Face(ftLib, fontPath, 0, &mainFace);
	assertf(err == 0, "Failed to create a new face for font \"%s\"", fontPath);

	if (!FT_IS_FIXED_WIDTH(mainFace))
		printf("Warning: Font face \"%s %s\" (%s) is not fixed width\n",
				mainFace->family_name, mainFace->style_name, fontPath);

	initShaders();

	setTextSize(nfontSize);

	setGlobalTransformation(0, 0);
}

void TextDrawer::BindCacher(TextCacher *ncacher, unsigned int rows,
		unsigned int columns)
{
	cacher = ncacher;
	cacher->face = mainFace;
	cacher->ftLib = ftLib;
	cacher->td = this;

	cacher->GetCellSizes();
	windowWidth = cacher->cellWidth*rows;
	windowHeight = cacher->cellWidth*columns;

	cacher->Precache();
}

void TextDrawer::initShaders()
{
#define GLSL(src) "#version 120\n" #src
	const char *ForegroundVertShaderSrc = GLSL(
		attribute vec2 inVertCoord;
		attribute vec2 inTextureCoord;
		varying vec2 outTextureCoord;

		uniform vec2 transformation;
		uniform vec2 globalTransformation;

		void main() {
			vec2 result = inVertCoord+transformation+globalTransformation;
			gl_Position = vec4(result, 0, 1);
			outTextureCoord = inTextureCoord;
		}
	);
	const char *ForegroundFragShaderSrc = GLSL(
		varying vec2 outTextureCoord;
		uniform sampler2D tex0;
		uniform vec3 fg;

		void main() {
			gl_FragColor = vec4(fg, texture2D(tex0, outTextureCoord).r);
		}
	);

	const char *BackgroundVertShaderSrc = GLSL(
		attribute vec2 inVertCoord;

		uniform vec2 transformation;
		uniform vec2 globalTransformation;

		void main() {
			vec2 result = inVertCoord+transformation+globalTransformation;
			gl_Position = vec4(result, 0, 1);
		}
	);
	const char *BacgroundFragShaderSrc = GLSL(
		uniform vec3 bg;

		void main() {
			gl_FragColor = vec4(bg, 1);
		}
	);
#undef GLSL

	fg_vertShader = CreateShader(GL_VERTEX_SHADER, ForegroundVertShaderSrc);
	fg_fragShader = CreateShader(GL_FRAGMENT_SHADER, ForegroundFragShaderSrc);
	fg_shaderProgram = CreateShaderProgram(fg_vertShader, fg_fragShader);

	fg_vertCoordAttribute = BindAttribute(fg_shaderProgram, "inVertCoord");
	fg_textureCoordAttribute = BindAttribute(fg_shaderProgram, "inTextureCoord");

	fg_textureUnif = BindUniform(fg_shaderProgram, "tex0");
	fg_FGcolorUnif = BindUniform(fg_shaderProgram, "fg");
	fg_transfUnif = BindUniform(fg_shaderProgram, "transformation");
	fg_gtransfUnif = BindUniform(fg_shaderProgram, "globalTransformation");


	bg_vertShader = CreateShader(GL_VERTEX_SHADER, BackgroundVertShaderSrc);
	bg_fragShader = CreateShader(GL_FRAGMENT_SHADER, BacgroundFragShaderSrc);
	bg_shaderProgram = CreateShaderProgram(bg_vertShader, bg_fragShader);

	bg_vertCoordAttribute = BindAttribute(bg_shaderProgram, "inVertCoord");

	bg_BGcolorUnif = BindUniform(bg_shaderProgram, "bg");
	bg_transfUnif = BindUniform(bg_shaderProgram, "transformation");
	bg_gtransfUnif = BindUniform(bg_shaderProgram, "globalTransformation");
}

float TextDrawer::RenderChar(const uint32_t ch, const float x, const float y)
{
	const glyph glyph = cacher->Lookup(ch);
	GLfloat transformation[2] = { x, y };

	// -------------------- background -----
	glUseProgram(bg_shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, cacher->bg_cellVertCoordsVBO);
	glEnableVertexAttribArray(bg_vertCoordAttribute);
	glVertexAttribPointer(bg_vertCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glUniform2fv(bg_transfUnif, 1, transformation);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(bg_vertCoordAttribute);

	// -------------------- foreground -----
	glUseProgram(fg_shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, glyph.fg_glyphVertCoordsVBO);
	glEnableVertexAttribArray(fg_vertCoordAttribute);
	glVertexAttribPointer(fg_vertCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cacher->fg_texCoordsVBO);
	glEnableVertexAttribArray(fg_textureCoordAttribute);
	glVertexAttribPointer(fg_textureCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindTexture(GL_TEXTURE_2D, glyph.textureID);

	glUniform2fv(fg_transfUnif, 1, transformation);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(fg_vertCoordAttribute);
	glDisableVertexAttribArray(fg_textureCoordAttribute);

	return glyph.xAdvance*sx;
}

void TextDrawer::setTextForeground(unsigned char r, unsigned char g, unsigned char b)
{
	glUseProgram(fg_shaderProgram);
	GLfloat color[3] = { r/255.f, g/255.f, b/255.f };
	glUniform3fv(fg_FGcolorUnif, 1, color);
}

void TextDrawer::setTextBackground(unsigned char r, unsigned char g, unsigned char b)
{
	glUseProgram(bg_shaderProgram);
	GLfloat color[3] = { r/255.f, g/255.f, b/255.f };
	glUniform3fv(bg_BGcolorUnif, 1, color);
}

void TextDrawer::setGlobalTransformation(float x, float y)
{
	GLfloat transf[2] = { x, -y };

	glUseProgram(fg_shaderProgram);
	glUniform2fv(fg_gtransfUnif, 1, transf);

	glUseProgram(bg_shaderProgram);
	glUniform2fv(bg_gtransfUnif, 1, transf);
}

void TextDrawer::setTextSize(unsigned int nfontSize)
{
	fontSize = nfontSize;
	FT_Set_Pixel_Sizes(mainFace, fontSize, fontSize);
}

TextDrawer::~TextDrawer()
{
	glDeleteProgram(fg_shaderProgram);
	glDeleteShader(fg_vertShader);
	glDeleteShader(fg_fragShader);

	glDeleteProgram(bg_shaderProgram);
	glDeleteShader(bg_vertShader);
	glDeleteShader(bg_fragShader);

	FT_Done_Face(mainFace);
	FT_Done_FreeType(ftLib);
}

