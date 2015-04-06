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
			row.ch = ' '; //(rand() % 25) + std::min('a', 'A') +
				// (rand() % 2)*(std::max('a', 'A')-std::min('a', 'A'));

	matrix[0][0].ch = 't';
	matrix[0][1].ch = 'l';

	matrix[0][rows-2].ch = 't';
	matrix[0][rows-1].ch = 'r';

	matrix[columns-1][0].ch = 'b';
	matrix[columns-1][1].ch = 'l';

	matrix[columns-1][rows-2].ch = 'b';
	matrix[columns-1][rows-1].ch = 'r';
}

