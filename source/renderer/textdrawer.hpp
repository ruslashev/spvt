#ifndef TEXTDRAWER_HPP
#define TEXTDRAWER_HPP

#include "textcacher.hpp"
#include "glutils.hpp"
#include "../errors.hpp"

#include <cstdio>
#include <map>
#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

class TextCacher;

class TextDrawer
{
	FT_Library ftLib;
	FT_Face mainFace;
	TextCacher *cacher;

	GLint fg_textureUnif, fg_FGcolorUnif, fg_transfUnif, fg_gtransfUnif;
	GLint fg_vertCoordAttribute, fg_textureCoordAttribute;
	GLuint fg_vertShader, fg_fragShader;
	GLuint fg_shaderProgram;

	GLint bg_BGcolorUnif, bg_transfUnif, bg_gtransfUnif;
	GLint bg_vertCoordAttribute;
	GLuint bg_vertShader, bg_fragShader;
	GLuint bg_shaderProgram;

	void initShaders();
public:
	TextDrawer(const char *fontPath);
	~TextDrawer();

	unsigned int fontHeight;
	const float sx, sy;
	const float lineSpacing;

	void RenderChar(const uint32_t ch, float &dx, const float dy);
	void setTextForeground(unsigned char r, unsigned char g, unsigned char b);
	void setTextBackground(unsigned char r, unsigned char g, unsigned char b);
	void setGlobalTransformation(float x, float y);
	void setTextSize(unsigned int size);
};

#endif

