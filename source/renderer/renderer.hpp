#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "textdrawer.hpp"
#include "../charmatrix.hpp"
#include "../errors.hpp"

class Renderer
{
	SDL_Window *_sdl_window;
	SDL_GLContext _glcontext;

	std::unique_ptr<TextDrawer> _textDrawer_ptr;
	std::unique_ptr<TextCacher> _textCacher_ptr;
	CharMatrix *_charMatrix_ptr;

	void init();
	void handleEvents();
	void renderStrings();
public:
	bool quit;

	Renderer(CharMatrix *ncharMatrix_ptr);
	~Renderer();

	void UpdateAndDraw();
};

#endif

