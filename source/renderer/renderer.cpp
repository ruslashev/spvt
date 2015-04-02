#include "renderer.hpp"

Renderer::Renderer(CharMatrix *ncharMatrix_ptr)
{
	init();

	quit = false;

	_textDrawer_ptr =
		std::unique_ptr<TextDrawer>(new TextDrawer("symlink-to-font"));
	_charMatrix_ptr = ncharMatrix_ptr;
}

void Renderer::init()
{
	assertf(SDL_Init(SDL_INIT_EVERYTHING) >= 0,
			"Failed to initialize SDL: %s", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	_sdl_window = SDL_CreateWindow("spvt",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assertf(_sdl_window != NULL, "Failed to open a window: %s", SDL_GetError());

	_glcontext = SDL_GL_CreateContext(_sdl_window);
	assertf(_glcontext != NULL,
			"Failed to create OpenGL rendering context: %s", SDL_GetError());
}

void Renderer::UpdateAndDraw()
{
	handleEvents();

	glViewport(0, 0, 800, 600);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderStrings();

	SDL_GL_SwapWindow(_sdl_window);
}

void Renderer::renderStrings()
{
	TextDrawer *td = _textDrawer_ptr.get();

	td->setTextSize(14);
	td->setTextForeground(0, 0, 0);
	td->setTextBackground(255, 255, 255);

	const float cellHeight = (int)(td->fontHeight*td->lineSpacing)*td->sy;

	std::vector<std::string> lines;
	lines.push_back("AKLJAE:LJKG");
	lines.push_back("yeh");
	lines.push_back("wut");

	for (size_t l = 0; l < lines.size(); l++) {
		float dx = -1;
		const float dy = 1 - l*cellHeight;
		const std::string srcLine = lines.at(l);

		int cx = 0;
		for (size_t c = 0; c < srcLine.length(); c++, cx++) {
			if (srcLine[c] != '\t')
				td->RenderChar(srcLine[c], dx, dy);
			else {
				const int tabsize = 4;
				const int spacesToInsert = tabsize - (cx % tabsize);
				td->setTextForeground(200, 200, 200);
				td->RenderChar('|', dx, dy);
				cx++;
				for (int i = 1; i < spacesToInsert; i++) {
					td->RenderChar('-', dx, dy);
					cx++;
				}
				cx--;
				td->setTextForeground(0, 0, 0);
			}
		}
	}
}

void Renderer::handleEvents()
{
	SDL_StartTextInput();
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			quit = true;
		if (event.type == SDL_KEYUP &&
				event.key.keysym.sym == SDLK_ESCAPE)
			quit = true;
		if (event.type == SDL_WINDOWEVENT_RESIZED)
			printf("window resize to %dx%d\n",
					event.window.data1,
					event.window.data2);
	}
}

Renderer::~Renderer()
{
	SDL_GL_DeleteContext(_glcontext);
	SDL_DestroyWindow(_sdl_window);
	SDL_Quit();
}

