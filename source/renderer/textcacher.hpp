#ifndef TEXTCACHER_HPP
#define TEXTCACHER_HPP

class Renderer;

#include "glutils.hpp"
#include "renderer.hpp"
#include "../errors.hpp"

#include <map>
#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

struct Glyph {
	GLuint fg_glyphVertCoordsVBO, textureID;
	long xAdvance;
	int left, top;
	unsigned int width, height;
};

class TextCacher
{
	std::map<uint32_t, Glyph> glyphMap;

	void createTextureCoords();
	void createBackgroundCell();
public:
	Renderer *renderer;
	FT_Face face;
	FT_Library ftLib;

	float cellHeight, cellWidth;

	GLuint fg_texCoordsVBO, bg_cellVertCoordsVBO;

	void GetCellSizes();
	void CreateSharedBuffers();
	void Precache();
	Glyph Lookup(uint32_t ch);
	~TextCacher();
};

#endif

