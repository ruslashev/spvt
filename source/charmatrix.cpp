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
			column.ch = (rand() % 25) + std::min('a', 'A') +
				(rand() % 2)*(std::max('a', 'A')-std::min('a', 'A'));
}

