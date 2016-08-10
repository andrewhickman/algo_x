#ifndef _sudoku_x_h_
#define _sudoku_x_h_

#include "matrix.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace sudoku_x {

const size_t MATRIX_W = 180;
const size_t MATRIX_H = 216;

inline int get_num(const char c) {
	return static_cast<int>(c - '1');
}

inline char get_char(const int n) {
	return static_cast<char>(n) + '1';
}

inline bool is_clue(const int n) {
	return 0 <= n and n < 6;
}

namespace constraints {

// defines a set of functions such that f(j1, i1, n1) == f(j2, i2, n2) if and
// only if the constraint is not satisified. Return values in range [0, 36) 

int cell(int j, int i, int) {
	return i * 6 + j;
}
int row(int, int i, int n) {
	return i * 6 + n;
}
int col(int j, int, int n) {
	return j * 6 + n;
}
int block(int j, int i, int n) {
	int block_num = 3 * (j / 3) + (i / 2);
	return block_num * 6 + n;
}
int cross(int j, int i, int n) {
	if (j == i) {
		return 7 * n;
	} else if (j + i == 5) {
		return 5 * n + 5;
	} else {
		return i * 6 + j;
	}
}

inline bool matrix(int j, int i) {
	int i_num = i % 6;
	int i_col = (i /= 6) % 6;
	int i_row = (i /= 6) % 6;

	int constraint_type = (j / 36) % 5;

	switch (constraint_type) {
		case 0:
			return j % 36 == cell(i_col, i_row, i_num);
		case 1:
			return j % 36 == row(i_col, i_row, i_num);
		case 2:
			return j % 36 == col(i_col, i_row, i_num);
		case 3:
			return j % 36 == block(i_col, i_row, i_num);
		case 4:
			return j % 36 == cross(i_col, i_row, i_num);
	}
	return false;	 // unused
}

} // namespace constraints

inline std::string format_solution(const std::vector<RowNode*>& solution) {
	std::string ret;
	if (solution.empty()) {
		return ret;
	}
	static int grid[6][6];
	for (RowNode* n : solution) {
		size_t i = n->num;
		size_t num = (i % 6);
		size_t col = (i /= 6) % 6;
		size_t row = (i /= 6) % 6;
		grid[row][col] = num;
	}
	for (size_t i = 0; i < 6; ++i) {
		for (size_t j = 0; j < 6; ++j) {
			ret += get_char(grid[i][j]);
		}
	}
	return ret;
}

inline void print_grid(const std::string& puzzle, std::ostream& os) {
	if (puzzle.size() != 36) {
		return;
	}
	os << "┌───┬───┐\n│";
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			char c = puzzle[i * 6 + j];
			os << (is_clue(get_num(c)) ? c : ' ');
			if (j % 3 == 2) { 
				os << "│";
			}
		}
		os << "\n";
		if (i == 1 or i == 3) {
			os << "├───┼───┤\n";
		}
		os << "│";
	}
	os << "\b└───┴───┘\n";
}

inline void solve(const std::string& puzzle) {
	/* Solve a sudoku puzzle given as a string of length 36*/
	std::cout << "Solving puzzle:\n";
	print_grid(puzzle, std::cout);
	SparseMatrix M(MATRIX_H, MATRIX_W,
		[puzzle](size_t j, size_t i) -> bool {
			int c = get_num(puzzle[i / 6]);
			if (is_clue(c) and size_t(c) != (i % 6)) {
				return false;
			} else {
				return constraints::matrix(j, i);
			}
		}
	);
	auto solutions = M.solve_all(); 
	for (auto sol : solutions) {
		std::cout << "Found solution:\n";
		print_grid(format_solution(sol), std::cout);
	}
	std::cout << "Total solutions found: " << solutions.size() << '\n';
}	

inline int solve_file(std::ifstream& infile, std::ofstream& outfile) {
	/* Faster than the basic solve routine for multiple puzzles */
	SparseMatrix M(MATRIX_H, MATRIX_W, constraints::matrix);
	int line_count = 0;
	std::vector<RowNode*> clues;
	for (std::string puzzle; std::getline(infile, puzzle);) {
		if (puzzle.size() != 36) {
			continue;
		}
		++line_count;
		// Get clues
		for (int i = 0; i < 36; ++i) {
			int c = get_num(puzzle[i]);
			if (is_clue(c)) {
				for (int num = 0; num < 6; ++num) {
					if (c != num) {
						clues.push_back(M.rows[6 * i + num]);
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

} // sudoku_x

#endif
