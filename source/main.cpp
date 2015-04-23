#include "charmatrix.hpp"
#include "renderer/renderer.hpp"

#include <fstream>

int main()
{
	try {
		CharMatrix charMatrix(80, 25);
		Renderer renderer(14, &charMatrix);

		while (1) {
			renderer.UpdateAndDraw();
			if (renderer.quit)
				break;
		}
	} catch (std::exception &e) {
		fprintf(stderr, "\x1b[31m" "ERROR" "\x1b[0m" " %s", e.what());
		return 1;
	}
	return 0;
}

