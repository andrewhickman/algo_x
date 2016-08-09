#ifndef _sudoku_h_
#define _sudoku_h_

#include "matrix.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
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

namespace constraints {

// defines a set of functions such that f(j1, i1, n1) == f(j2, i2, n2) if and
// only if the constraint is not satisfied. Return values in range [0, 81) 

int cell(int j, int i, int) {
        return i * 9 + j;
}
int row(int, int i, int n) {
        return i * 9 + n;
}
int col(int j, int, int n) {
        return j * 9 + n;
}
int block(int j, int i, int n) {
        int block_num = 3 * (j / 3) + (i / 3);
        return block_num * 9 + n;
}

inline bool matrix(int j, int i) {
	int i_num = i % 9;
	int i_col = (i /= 9) % 9;
	int i_row = (i /= 9) % 9;

	int constraint_type = (j / 81) % 4;

	switch (constraint_type) {
		case 0:  
			return j % 81 == cell(i_col, i_row, i_num);
		case 1:
			return j % 81 == row(i_col, i_row, i_num);
                case 2:
                        return j % 81 == col(i_col, i_row, i_num);
                case 3:
                        return j % 81 == block(i_col, i_row, i_num);

	}
	return false;    // unused
}

} // namespace constraints

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

inline void print_grid(const std::string& puzzle, std::ostream& os) {
	if (puzzle.size() != 81) {
		return;
	}
	os << "┌───┬───┬───┐\n│";
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			char c = puzzle[i * 9 + j];
			os << (is_clue(get_num(c)) ? c : ' ');
			if (j % 3 == 2) { 
				os << "│";
			}
		}
		os << "\n";
		if (i == 2 or i == 5) {
			os << "├───┼───┼───┤\n";
		}
		os << "│";
	}
	os << "\b└───┴───┴───┘\n";
}

inline void solve(const std::string& puzzle) {
	/* Solve a sudoku puzzle given as a string of length 81 */
	std::cout << "Solving puzzle:\n";
	print_grid(puzzle, std::cout);
	SparseMatrix M(MATRIX_H, MATRIX_W,
		[puzzle](size_t j, size_t i) -> bool {
			int c = get_num(puzzle[i / 9]);
			if (is_clue(c) and size_t(c) != (i % 9)) {
				return false;
			} else {
				return constraints::matrix(j, i);
			}
		}
	);
	auto solutions = M.solve_all();
	if (solutions.empty()) {
		std::cout << "No solution found.\n";
	} else {
		for (auto sol : solutions) {
			std::cout << "Found solution:\n";
			print_grid(format_solution(sol), std::cout);
		}
	}
}	

inline int solve_file(std::ifstream& infile, std::ofstream& outfile) {
	/* Faster than the basic solve routine for multiple puzzles */
	SparseMatrix M(MATRIX_H, MATRIX_W, constraints::matrix);
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

#endif

