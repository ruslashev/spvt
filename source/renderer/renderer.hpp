#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

class TextCacher;

#include "textcacher.hpp"
#include "../charmatrix.hpp"
#include "../errors.hpp"

class Renderer
{
	SDL_Window *_sdl_window;
	SDL_GLContext _glcontext;

	FT_Library ftLib;
	FT_Face mainFace;

	GLint fg_textureUnif, fg_FGcolorUnif, fg_transfUnif, fg_gtransfUnif;
	GLint fg_vertCoordAttribute, fg_textureCoordAttribute;
	GLuint fg_vertShader, fg_fragShader;
	GLuint fg_shaderProgram;

	GLint bg_BGcolorUnif, bg_transfUnif, bg_gtransfUnif;
	GLint bg_vertCoordAttribute;
	GLuint bg_vertShader, bg_fragShader;
	GLuint bg_shaderProgram;

	std::unique_ptr<TextCacher> _textCacher_ptr;
	CharMatrix *_charMatrix_ptr;

	void init_SDL();
	void init_OpenGL();
	void init_fonts(const char* fontPath);
	void init_shaders();

	void handleEvents();
	void renderStrings();

	float RenderChar(const uint32_t ch, const float x, const float y);
	void setTextForeground(unsigned char r, unsigned char g, unsigned char b);
	void setTextBackground(unsigned char r, unsigned char g, unsigned char b);
	void setGlobalTransformation(float x, float y);
	void setTextSize(unsigned int nfontSize);
	void resizeWindow();
public:
	unsigned int windowWidth, windowHeight;
	unsigned int fontSize;
	float sx, sy;
	float lineSpacing;

	bool quit;

	Renderer(unsigned int nfontSize, CharMatrix *ncharMatrix_ptr);
	~Renderer();

	void UpdateAndDraw();
};

#endif

