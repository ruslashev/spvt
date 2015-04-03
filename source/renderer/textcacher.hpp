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

struct glyphKey {
	uint32_t ch;
	unsigned int size;
	bool operator<(const glyphKey &other) const {
		return ch < other.ch;
	}
};

class TextDrawer;

class TextCacher
{
	std::map<glyphKey, glyph> normalGlyphs;
public:
	FT_Face face;
	FT_Library ftLib;
	TextDrawer *td;

	GLuint fg_texCoordsVBO, bg_cellVertCoordsVBO;

	void Precache(unsigned int size);
	glyph Lookup(uint32_t ch, unsigned int size);
	~TextCacher();
};

#endif

