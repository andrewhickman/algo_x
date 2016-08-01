#include "matrix.h"

#include <string>

inline bool sudoku_constraints(size_t j, size_t i) {
	size_t inum = i % 9;
	size_t icol = (i /= 9) % 9;
	size_t irow = (i /= 9) % 9;

	size_t jx = j % 9;
	size_t jy = (j /= 9) % 9;
	size_t constraint_type = (j /= 9) % 4;

	switch (constraint_type) {
		case 0:    // cell constraints
			return (icol == jx) and (irow == jy);
		case 1:    // row constraints
			return (inum == jx) and (irow == jy);
		case 2:    // column constraints
			return (inum == jx) and (icol == jy);
		case 3:    // block constraints
			return (inum == jx) and (irow / 3 == jy % 3) and (icol / 3 == jy / 3);
	}
	return false;    // unused
}

inline std::string sudoku_solve(std::string puzzle) {
	SparseMatrix A(729, 324,
		[puzzle](size_t j, size_t i) -> bool {
			char clue = puzzle[i / 9];
			if ('1' <= clue and clue <= '9' and
				puzzle[i / 9] != static_cast<char>(i % 9) + '1') {
				return false;
			} else {
				return sudoku_constraints(j, i);
			}
		}
	);
	std::vector<RowNode*> solution = A.solve();
	std::string ret;
	int grid[9][9];
	for (RowNode* n : solution) {
		size_t i = n->num;
		size_t num = (i % 9) + 1;
		size_t col = (i /= 9) % 9;
		size_t row = (i /= 9) % 9;
		grid[row][col] = num;
	}
	for (size_t i = 0; i < 9; ++i) {
		for (size_t j = 0; j < 9; ++j) {
			ret += (char(grid[i][j]) + '0');
		}
	}
	return ret;
}
