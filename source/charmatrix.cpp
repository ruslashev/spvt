#include "charmatrix.hpp"

CharMatrix::CharMatrix(unsigned int nrows, unsigned int ncolumns)
{
	rows = nrows;
	columns = ncolumns;

	matrix.resize(columns);
	for (auto &column : matrix)
		column.resize(rows);

	for (auto &column : matrix)
		for (auto &row : column)
			row.ch = (rand() % 25) + std::min('a', 'A') +
				(rand() % 2)*(std::max('a', 'A')-std::min('a', 'A'));
}

