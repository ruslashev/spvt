#ifndef CHAR_MATRIX_HPP
#define CHAR_MATRIX_HPP

#include <vector>

#include "errors.hpp"

struct Cell
{
	char ch;
};

class CharMatrix
{
public:
	std::vector<std::vector<Cell>> matrix;
	unsigned int rows, columns;

	CharMatrix(unsigned int nrows, unsigned int ncolumns);
};

#endif


