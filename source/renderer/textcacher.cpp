#include "textcacher.hpp"

void TextCacher::GetCellSizes()
{
	const int err = FT_Load_Char(face, ' ', FT_LOAD_RENDER);
	assertf(err == 0, "Failed to render space character");
	cellWidth = face->glyph->advance.x >> 6;
	cellHeight = renderer->fontSize*renderer->lineSpacing;
}

void TextCacher::CreateSharedBuffers()
{
	createTextureCoords();
	createBackgroundCell();
}

void TextCacher::createTextureCoords()
{
	const GLfloat texCoords[4][2] = {
		{ 0, 0 },
		{ 1, 0 },
		{ 0, 1 },
		{ 1, 1 }
	};
	glGenBuffers(1, &fg_texCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fg_texCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
}

void TextCacher::createBackgroundCell()
{
	GLfloat bgCellVertCoords[4][2] = {
		{ 0,                        0 },
		{ 0+cellWidth*renderer->sx, 0 },
		{ 0,                        0-cellHeight*renderer->sy },
		{ 0+cellWidth*renderer->sx, 0-cellHeight*renderer->sy }
	};
	glGenBuffers(1, &bg_cellVertCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bg_cellVertCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgCellVertCoords), bgCellVertCoords,
			GL_STATIC_DRAW);
}

void TextCacher::Precache()
{
	for (int i = 0; i < 256; i++)
		Lookup(i);
}

Glyph TextCacher::Lookup(uint32_t ch)
{
	if (glyphMap.find(ch) != glyphMap.end()) {
		return glyphMap.at(ch);
	} else {
		// doesn't exist and needs to be drawn and written
		const int err = FT_Load_Char(face, ch, FT_LOAD_RENDER);
		assertf(err == 0, "Failed to render char '%c' %d 0x%x", ch, ch, ch);

		const float sx = renderer->sx;
		const float sy = renderer->sy;
		const FT_GlyphSlot g = face->glyph;
		const float x2 = g->bitmap_left*sx;
		const float y2 = g->bitmap_top*sy - renderer->fontSize*sy;
		const float w  = g->bitmap.width*sx;
		const float h  = g->bitmap.rows*sy;
		GLfloat fgVertCoords[4][2] = {
			{ x2,   y2   },
			{ x2+w, y2   },
			{ x2,   y2-h },
			{ x2+w, y2-h }
		};
		GLuint fg_glyphVertCoordsVBO;
		glGenBuffers(1, &fg_glyphVertCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, fg_glyphVertCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(fgVertCoords), fgVertCoords,
				GL_STATIC_DRAW);

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				g->bitmap.width, g->bitmap.rows, 0,
				GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		const Glyph value = {
			fg_glyphVertCoordsVBO,
			textureID,
			g->advance.x >> 6,
			g->bitmap_left,
			g->bitmap_top,
			g->bitmap.width,
			g->bitmap.rows
		};

		glyphMap[ch] = value;

		return value;
	}
}

TextCacher::~TextCacher()
{
	for (auto it = glyphMap.begin(); it != glyphMap.end(); ++it) {
		glDeleteBuffers(1, &it->second.fg_glyphVertCoordsVBO);
		glDeleteTextures(1, &it->second.textureID);
	}
	glDeleteBuffers(1, &fg_texCoordsVBO);
	glDeleteBuffers(1, &bg_cellVertCoordsVBO);
}

