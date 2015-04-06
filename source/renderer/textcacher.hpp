#ifndef TEXTCACHER_HPP
#define TEXTCACHER_HPP

#include "textdrawer.hpp"
#include "glutils.hpp"
#include "../errors.hpp"

#include <cstdio>
#include <map>
#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

struct glyph {
	GLuint fg_glyphVertCoordsVBO, textureID;
	long xAdvance;
	int left, top;
	unsigned int width, height;
};

class TextDrawer;

class TextCacher
{
	std::map<uint32_t, glyph> glyphMap;

	void precacheTextureCoords();
	void precacheBackgroundCell();
public:
	FT_Face face;
	FT_Library ftLib;
	TextDrawer *td;

	float cellHeight, cellWidth;

	GLuint fg_texCoordsVBO, bg_cellVertCoordsVBO;

	void GetCellSizes();
	void Precache();
	glyph Lookup(uint32_t ch);
	~TextCacher();
};

#endif

