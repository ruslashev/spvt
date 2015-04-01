#include "texteditor.hpp"

TextEditor::TextEditor(const char *fontPath) : td(fontPath)
{
}

void TextEditor::Draw()
{
	td.setTextSize(14);
	td.setTextForeground(0, 0, 0);
	td.setTextBackground(255, 255, 255);

	RenderFile();
}

void TextEditor::RenderFile()
{
	const float cellHeight = (int)(td.fontHeight*td.lineSpacing)*td.sy;

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
				td.RenderChar(srcLine[c], dx, dy);
			else {
				const int tabsize = 4;
				const int spacesToInsert = tabsize - (cx % tabsize);
				td.setTextForeground(200, 200, 200);
				td.RenderChar('|', dx, dy);
				cx++;
				for (int i = 1; i < spacesToInsert; i++) {
					td.RenderChar('-', dx, dy);
					cx++;
				}
				cx--;
				td.setTextForeground(0, 0, 0);
			}
		}
	}
}

