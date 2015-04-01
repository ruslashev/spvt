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
	SDL_Event event;

	std::unique_ptr<TextDrawer> _textDrawer_ptr;
public:
	void Create();
	~Renderer();

	void Draw();
	void RenderStrings();
	char getch();
};

#endif

