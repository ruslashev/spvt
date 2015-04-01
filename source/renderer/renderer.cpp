#include "renderer.hpp"

void Renderer::Create()
{
	assertf(SDL_Init(SDL_INIT_EVERYTHING) >= 0,
			"Failed to initialize SDL: %s", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);

	_sdl_window = SDL_CreateWindow("spvt",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assertf(_sdl_window != NULL, "Failed to open a window: %s", SDL_GetError());

	_glcontext = SDL_GL_CreateContext(_sdl_window);
	assertf(_glcontext != NULL,
			"Failed to create OpenGL rendering context: %s", SDL_GetError());

	_textDrawer_ptr = std::unique_ptr<TextDrawer>(
			new TextDrawer("symlink-to-font"));
}

void Renderer::Draw()
{
	glViewport(0, 0, 800, 600);

	const float grayShade = 0.075f;
	glClearColor(grayShade, grayShade, grayShade, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RenderStrings();

	SDL_GL_SwapWindow(_sdl_window);
}

void Renderer::RenderStrings()
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
	SDL_GL_DeleteContext(_glcontext);
	SDL_DestroyWindow(_sdl_window);
	SDL_Quit();
}

