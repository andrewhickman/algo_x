#ifndef _sudoku_h_
#define _sudoku_h_

#include "digit.h"
#include "matrix.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*  
 *  Allows solving of 9x9 and 6x6 sudokus with optional cross rule
 */

namespace sudoku {

namespace {

enum RowDigits {
    row, col, num
};
enum ColDigits {
    type, val    
};

inline int get_num(const char c) {
	return static_cast<int>(c - '1');
}
inline char get_char(const int n) {
	return static_cast<char>(n) + '1';
}
template<int sz>
inline bool is_clue(const int n) {
	return 0 <= n and n < sz;
}

/*  
 *  Defines a set of functions such that f(j1, i1, n1) == f(j2, i2, n2) if and
 *  only if the constraint is not satisfied. Return values in range [0, sz^3).
 *  Note for odd sz values two functions are necessary for the cross constraint.
 */

template<int sz>
inline int cell_constraint(int j, int i, int) {
	return i * sz + j;
}
template<int sz>
inline int row_constraint(int, int i, int n) {
	return i * sz + n;
}
template<int sz>
inline int col_constraint(int j, int, int n) {
	return j * sz + n;
}
template<int sz>
inline int block_constraint(int j, int i, int n) {
	static int block_width = (sz == 9 ? 3 : 2);
	int block_num = 3 * (j / 3) + (i / block_width);
	return block_num * sz + n;
}
template<int sz>
inline int cross_constraint_1(int j, int i, int n) {
	if (j == i) {
		return (sz + 1) * n;
	}
	return j * sz + i;
}
template<int sz>
inline int cross_constraint_2(int j, int i, int n) {
	if (j + i == sz - 1) {
		return (sz - 1) * (n + 1);
	}
	return j * sz + i; 
}

using ConstraintType = int (*)(int, int, int);

template<int sz>
const ConstraintType constraints[] = {
    cell_constraint<sz>,
    row_constraint<sz>,
    col_constraint<sz>,
    block_constraint<sz>,
    cross_constraint_1<sz>,
    cross_constraint_2<sz>,
};

template<int sz>
inline bool constraints_matrix(int j_, int i_) {
    DigitInt<sz, sz, sz> i(i_);
    DigitInt<6, sz * sz> j(j_);

    return j[val] == constraints<sz>[j[type]](i[col], i[row], i[num]);
}

}

template<int sz>
inline std::string format_solution(const std::vector<HeadNode*>& solution) {
	std::string ret;
	if (solution.empty()) {
		return ret;
	}
	static int grid[sz][sz];
	for (HeadNode* n : solution) {
        DigitInt<9, 9, 9> data(n->data);
		grid[data.get(row)][data.get(col)] = data.get(num);
	}
	for (size_t i = 0; i < sz; ++i) {
		for (size_t j = 0; j < sz; ++j) {
			ret += get_char(grid[i][j]);
		}
	}
	return ret;
}

template<int sz>
inline void print_grid(const std::string& puzzle, std::ostream& os);

template<>
inline void print_grid<9>(const std::string& puzzle, std::ostream& os) {
	os << "┌───┬───┬───┐\n│";
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			char c = puzzle[i * 9 + j];
			os << (is_clue<9>(get_num(c)) ? c : ' ');
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

template<>
inline void print_grid<6>(const std::string& puzzle, std::ostream& os) {
	os << "┌───┬───┐\n│";
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			char c = puzzle[i * 6 + j];
			os << (is_clue<6>(get_num(c)) ? c : ' ');
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

template<int sz, bool use_cross_rule = false>
inline void solve(const std::string& puzzle) {
	/* Solve a sudoku puzzle given as a string of length sz**2 */
	std::cout << "Solving puzzle:\n";
	print_grid<sz>(puzzle, std::cout);
	int height = sz * sz * sz;
	int width = sz * sz * (use_cross_rule ? 6 : 4);
	SparseMatrix M(height, width,
		[puzzle](size_t j, size_t i) -> bool {
			int c = get_num(puzzle[i / sz]);
			if (is_clue<sz>(c) and size_t(c) != (i % sz)) {
				return false;
			}
			return constraints_matrix<sz>(j, i);
		}
	);
	auto solutions = M.solve_all();
	if (solutions.size() == 1) {
		std::cout << "Found solution:\n";
		print_grid<sz>(format_solution<sz>(solutions.back()), std::cout);
	}
	std::cout << "Total solutions found: " << solutions.size() << '\n';
}

template<int sz, bool use_cross_rule = false>
inline int solve_file(std::ifstream& infile, std::ofstream& outfile) {
	/* Faster than the basic solve routine for multiple puzzles */
	const int sz2 = sz * sz;
	SparseMatrix M(sz2 * sz, sz2 * (use_cross_rule ? 6 : 4),
	               constraints_matrix<sz>);
	int line_count = 0;
	for (std::string puzzle; std::getline(infile, puzzle);) {
	    std::vector<HeadNode*> clues;
		if (puzzle.size() != sz2) {
			continue;
		}
		++line_count;
		// Get clues
		for (int i = 0; i < sz2; ++i) {
			int c = get_num(puzzle[i]);
			if (is_clue<sz>(c)) {
				for (int num = 0; num < sz; ++num) {
					if (c != num) {
						clues.push_back(M.rows[sz * i + num]);
					}
				}
			}
		}
		// Remove rows corresponding to clues
		for (auto it = clues.begin(); it != clues.end(); ++it) {
			M.remove_row(*it);
		}
		outfile << format_solution<sz>(M.solve()) << '\n';
		// Replace removed rows in reverse order
		for (auto it = clues.rbegin(); it != clues.rend(); ++it) {
			M.replace_row(*it);
		}
	}
	return line_count;
}

} // namespace sudoku

#endif

