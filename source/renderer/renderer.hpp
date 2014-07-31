#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL.h>

class Renderer
{
	SDL_Window *window;
	SDL_GLContext ctxt;
	SDL_Event event;

public:
	void Create();
	~Renderer();

	void Draw();
	char getch();
};

#endif

