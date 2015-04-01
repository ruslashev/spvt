#include "renderer/renderer.hpp"
#include <fstream>

int main()
{
	Renderer rend;

	try {
		rend.Create();
	} catch (std::exception &e) {
		fprintf(stderr, "\x1b[31m" "INIT ERROR" "\x1b[0m" " %s\n", e.what());
		return 1;
	}

	try {
		while (1) {
			rend.Draw();
			rend.getch();
		}
	} catch (std::exception &e) {
		fprintf(stderr, "\x1b[31m" "RUNTIME ERROR" "\x1b[0m" " %s\n", e.what());
		return 2;
	}

	return 0;
}

