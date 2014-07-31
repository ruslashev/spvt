#include "renderer.hpp"
#include "../errors.hpp"
#include "widgets/texteditor.hpp"
#include "widgets/statusbar.hpp"

void Renderer::Create(Editor *nep)
{
	ep = nep;

	// Initialize SDL
	int errCode = SDL_Init(SDL_INIT_EVERYTHING) < 0;
	assertf(errCode >= 0, "Failed to initialize SDL: %s", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);

	// Create window
	window = SDL_CreateWindow("Spvt",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assertf(window != NULL, "Failed to open a window: %s", SDL_GetError());

	// Initialize SDL_GL
	ctxt = SDL_GL_CreateContext(window);
	assertf(ctxt != NULL, "Failed to create OpenGL rendering context: %s", SDL_GetError());
	if (SDL_GL_SetSwapInterval(1) < 0)
		printf("Warning: Unable to set VSync: %s\n", SDL_GetError());
}

void Renderer::Draw()
{
	glViewport(0, 0, 800, 600);

	const float grayShade = 0.075f;
	glClearColor(grayShade, grayShade, grayShade, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SwapWindow(window);
}

char Renderer::getch()
{
	SDL_StartTextInput();
	while (1) {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_TEXTINPUT) {
				return event.text.text[0];
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_TAB:
						return 9;
						break;
					default:
						break;
				}
			}
		}
	}
}

Renderer::~Renderer()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

