#ifndef TEXTEDITOR_HPP
#define TEXTEDITOR_HPP

#include <vector>
#include <string>

#include "../textdrawer.hpp"

class TextEditor
{
	TextDrawer td;
	void RenderFile();
public:
	TextEditor(const char *fontPath);

	void Draw();
};

#endif

