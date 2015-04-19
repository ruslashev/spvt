#include "renderer/renderer.hpp"
#include "charmatrix.hpp"
#include <fstream>

int main()
{
	try {
		CharMatrix cm(80, 25);
		Renderer rend(28, &cm);

		while (1) {
			rend.UpdateAndDraw();
			if (rend.quit)
				break;
		}
	} catch (std::exception &e) {
		fprintf(stderr, "\x1b[31m" "ERROR" "\x1b[0m" " %s", e.what());
		return 1;
	}
	return 0;
}

