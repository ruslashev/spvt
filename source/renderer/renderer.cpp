#include "renderer.hpp"

Renderer::Renderer(unsigned int nfontSize, CharMatrix *ncharMatrix_ptr)
{
	init_SDL();
	init_OpenGL();
	init_fonts("symlink-to-font");
	init_shaders();

	lineSpacing = 1.35f;

	quit = false;

	setTextSize(nfontSize);
	setGlobalTransformation(0, 0);

	charMatrix_ptr = ncharMatrix_ptr;

	textCacher_ptr =
		std::unique_ptr<TextCacher>(new TextCacher);
	textCacher_ptr->face = mainFace;
	textCacher_ptr->ftLib = ftLib;
	textCacher_ptr->renderer = this;
	textCacher_ptr->GetCellSizes();
	windowWidth = textCacher_ptr->cellWidth*charMatrix_ptr->rows;
	windowHeight = textCacher_ptr->cellHeight*charMatrix_ptr->columns;
	sx = 2.f / windowWidth;
	sy = 2.f / windowHeight;
	textCacher_ptr->CreateSharedBuffers();
	textCacher_ptr->Precache();
	resizeWindow();
}

void Renderer::init_SDL()
{
	assertf(SDL_Init(SDL_INIT_EVERYTHING) >= 0,
			"Failed to initialize SDL: %s", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	_sdl_window = SDL_CreateWindow("spvt",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			100, 100,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assertf(_sdl_window != NULL,
			"Failed to open a window: %s", SDL_GetError());

	_glcontext = SDL_GL_CreateContext(_sdl_window);
	assertf(_glcontext != NULL,
			"Failed to create OpenGL rendering context: %s", SDL_GetError());
}

void Renderer::init_OpenGL()
{
	GLenum err = glewInit();
	assertf(err == GLEW_OK,
			"Failed to initialize GLEW: %s", glewGetErrorString(err));
	assertf(GLEW_VERSION_2_1,
			"Your graphics card's OpenGL version is less than 2.1.");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::init_fonts(const char* fontPath)
{
	assertf(FT_Init_FreeType(&ftLib) == 0,
			"Failed to initialize FreeType");

	assertf(FT_New_Face(ftLib, fontPath, 0, &mainFace) == 0,
			"Failed to create a new face for font \"%s\"", fontPath);

	if (!FT_IS_FIXED_WIDTH(mainFace))
		printf("Warning: Font face \"%s %s\" (%s) is not fixed width\n",
				mainFace->family_name, mainFace->style_name, fontPath);
}

void Renderer::init_shaders()
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
		uniform sampler2D glyphTexture;
		uniform vec3 fg_color;

		void main() {
			gl_FragColor = vec4(fg_color, texture2D(glyphTexture, outTextureCoord).r);
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
		uniform vec3 bg_color;

		void main() {
			gl_FragColor = vec4(bg_color, 1);
		}
	);
#undef GLSL

	fg_vertShader = CreateShader(GL_VERTEX_SHADER, ForegroundVertShaderSrc);
	fg_fragShader = CreateShader(GL_FRAGMENT_SHADER, ForegroundFragShaderSrc);
	fg_shaderProgram = CreateShaderProgram(fg_vertShader, fg_fragShader);

	fg_vertCoordAttribute = BindAttribute(fg_shaderProgram, "inVertCoord");
	fg_textureCoordAttribute = BindAttribute(fg_shaderProgram, "inTextureCoord");

	fg_textureUnif = BindUniform(fg_shaderProgram, "glyphTexture");
	fg_FGcolorUnif = BindUniform(fg_shaderProgram, "fg_color");
	fg_transfUnif = BindUniform(fg_shaderProgram, "transformation");
	fg_gtransfUnif = BindUniform(fg_shaderProgram, "globalTransformation");


	bg_vertShader = CreateShader(GL_VERTEX_SHADER, BackgroundVertShaderSrc);
	bg_fragShader = CreateShader(GL_FRAGMENT_SHADER, BacgroundFragShaderSrc);
	bg_shaderProgram = CreateShaderProgram(bg_vertShader, bg_fragShader);

	bg_vertCoordAttribute = BindAttribute(bg_shaderProgram, "inVertCoord");

	bg_BGcolorUnif = BindUniform(bg_shaderProgram, "bg_color");
	bg_transfUnif = BindUniform(bg_shaderProgram, "transformation");
	bg_gtransfUnif = BindUniform(bg_shaderProgram, "globalTransformation");
}

void Renderer::UpdateAndDraw()
{
	handleEvents();

	glViewport(0, 0, windowWidth, windowHeight);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	renderStrings();

	SDL_GL_SwapWindow(_sdl_window);
}

void Renderer::renderStrings()
{
	setTextForeground(0, 0, 0);
	setTextBackground(255, 255, 255);

	int line = 0;
	for (auto &row : charMatrix_ptr->matrix) {
		float x = -1;
		const float y = 1 - line*textCacher_ptr->cellHeight*sy;
		for (auto &column : row) {
			RenderChar(column.ch, x, y);
			x += textCacher_ptr->cellWidth*sx;
		}
		line++;
	}
}

void Renderer::handleEvents()
{
	// SDL_StartTextInput();
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			quit = true;
		if (event.type == SDL_KEYUP &&
				event.key.keysym.sym == SDLK_ESCAPE)
			quit = true;
		if (event.type == SDL_WINDOWEVENT_RESIZED)
			printf("window resize to %dx%d\n",
					event.window.data1,
					event.window.data2);
	}
}

void Renderer::RenderChar(const uint32_t ch, const float x, const float y)
{
	const Glyph glyph = textCacher_ptr->Lookup(ch);
	GLfloat transformation[2] = { x, y };

	// -------------------- background -----
	glUseProgram(bg_shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, textCacher_ptr->bg_cellVertCoordsVBO);
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

	glBindBuffer(GL_ARRAY_BUFFER, textCacher_ptr->fg_texCoordsVBO);
	glEnableVertexAttribArray(fg_textureCoordAttribute);
	glVertexAttribPointer(fg_textureCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindTexture(GL_TEXTURE_2D, glyph.textureID);

	glUniform2fv(fg_transfUnif, 1, transformation);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(fg_vertCoordAttribute);
	glDisableVertexAttribArray(fg_textureCoordAttribute);
}

void Renderer::setTextForeground(unsigned char r, unsigned char g,
		unsigned char b)
{
	glUseProgram(fg_shaderProgram);
	GLfloat color[3] = { r/255.f, g/255.f, b/255.f };
	glUniform3fv(fg_FGcolorUnif, 1, color);
}

void Renderer::setTextBackground(unsigned char r, unsigned char g,
		unsigned char b)
{
	glUseProgram(bg_shaderProgram);
	GLfloat color[3] = { r/255.f, g/255.f, b/255.f };
	glUniform3fv(bg_BGcolorUnif, 1, color);
}

void Renderer::setGlobalTransformation(float x, float y)
{
	GLfloat transf[2] = { x, y };

	glUseProgram(fg_shaderProgram);
	glUniform2fv(fg_gtransfUnif, 1, transf);

	glUseProgram(bg_shaderProgram);
	glUniform2fv(bg_gtransfUnif, 1, transf);
}

void Renderer::setTextSize(unsigned int nfontSize)
{
	fontSize = nfontSize;
	FT_Set_Pixel_Sizes(mainFace, fontSize, fontSize);
}

void Renderer::resizeWindow()
{
	SDL_SetWindowSize(_sdl_window, windowWidth, windowHeight);
}

Renderer::~Renderer()
{
	glDeleteProgram(fg_shaderProgram);
	glDeleteShader(fg_vertShader);
	glDeleteShader(fg_fragShader);

	glDeleteProgram(bg_shaderProgram);
	glDeleteShader(bg_vertShader);
	glDeleteShader(bg_fragShader);

	FT_Done_Face(mainFace);
	FT_Done_FreeType(ftLib);

	SDL_GL_DeleteContext(_glcontext);
	SDL_DestroyWindow(_sdl_window);
	SDL_Quit();
}

