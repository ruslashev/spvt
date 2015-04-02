#include "renderer/renderer.hpp"
#include <fstream>

int main()
{
	Renderer rend;

	try {
		rend.Create();
		while (1) {
			rend.UpdateAndDraw();
		}
	} catch (std::exception &e) {
		fprintf(stderr, "\x1b[31m" "ERROR" "\x1b[0m" " %s", e.what());
		return 1;
	}

	return 0;
}

