#include "charmatrix.hpp"

CharMatrix::CharMatrix(unsigned int nrows, unsigned int ncols)
{
	rows = nrows;
	cols = ncols;

	matrix.resize(rows);
	for (auto &row : matrix)
		row.resize(cols);

	for (auto &row : matrix)
		for (auto &column : row)
			column.ch = (rand() % 27) + std::min('a', 'A');
}

