#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL.h>

#include "../errors.hpp"
#include "textdrawer.hpp"

class Renderer
{
	SDL_Window *_sdl_window;
	SDL_GLContext _glcontext;

	std::unique_ptr<TextDrawer> _textDrawer_ptr;
	char handleEvents();
	void renderStrings();
public:
	void Create();
	~Renderer();

	void UpdateAndDraw();
};

#endif

