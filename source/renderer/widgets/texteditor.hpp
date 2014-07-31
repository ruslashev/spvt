#ifndef TEXTEDITOR_HPP
#define TEXTEDITOR_HPP

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

