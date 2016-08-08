#include "matrix.h"

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace sudoku {

const size_t MATRIX_W = 324;
const size_t MATRIX_H = 729;

inline int get_num(const char c) {
	return static_cast<int>(c - '1');
}

inline char get_char(const int n) {
	return static_cast<char>(n) + '1';
}

inline bool is_clue(const int n) {
	return 0 <= n and n < 9;
}

inline bool constraints(size_t j, size_t i) {
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

inline std::string format_solution(const std::vector<RowNode*>& solution) {
	std::string ret;
	if (solution.empty()) {
		return ret;
	}
	static int grid[9][9];
	for (RowNode* n : solution) {
		size_t i = n->num;
		size_t num = (i % 9);
		size_t col = (i /= 9) % 9;
		size_t row = (i /= 9) % 9;
		grid[row][col] = num;
	}
	for (size_t i = 0; i < 9; ++i) {
		for (size_t j = 0; j < 9; ++j) {
			ret += get_char(grid[i][j]);
		}
	}
	return ret;
}

inline std::string solve(const std::string& puzzle) {
	/* Solve a sudoku puzzle given as a string of length 81 */
	SparseMatrix M(MATRIX_H, MATRIX_W,
		[puzzle](size_t j, size_t i) -> bool {
			int c = get_num(puzzle[i / 9]);
			if (is_clue(c) and c != (i % 9)) {
				return false;
			} else {
				return constraints(j, i);
			}
		}
	);
	return format_solution(M.solve());
}

inline int solve_file(std::ifstream& infile, std::ofstream& outfile) {
	/* Faster than the basic solve routine for multiple puzzles */
	SparseMatrix M(MATRIX_H, MATRIX_W, constraints);
	int line_count = 0;
	std::vector<RowNode*> clues;
	for (std::string puzzle; std::getline(infile, puzzle);) {
		if (puzzle.size() != 81) {
			continue;
		}
		++line_count;
		// Get clues
		for (int i = 0; i < 81; ++i) {
			int c = get_num(puzzle[i]);
			if (is_clue(c)) {
				for (int num = 0; num < 9; ++num) {
					if (c != num) {
						clues.push_back(M.rows[9 * i + num]);
					}
				}
			}
		}
		// Remove rows corresponding to clues
		for (auto it = clues.begin(); it != clues.end(); ++it) {
			M.remove_row(*it);
		}
		outfile << format_solution(M.solve()) << '\n';
		// Replace removed rows in reverse order
		for (auto it = clues.rbegin(); it != clues.rend(); ++it) {
			M.replace_row(*it);
		}
		clues.clear();
	}
	return line_count;
}

} // namespace sudoku
