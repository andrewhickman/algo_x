#include "sudoku.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	std::string puzzle = argc > 1 ? argv[argc-1] : "";
	bool use_x = false;
	switch (argc) {
		default:
		case 3:
			if (argv[1][0] == '-' and argv[1][1] == 'x') {
				use_x = true;
			}
			break;
		case 2:
			break;
		case 1:
		case 0:
			std::cout << "usage: sudoku [options] <puzzle>\n";
			std::cout << "   x          Use cross rule\n";
			return 0;
	}
	if (puzzle.size() == 81) {
		if (use_x) {
			sudoku::solve<9, true>(puzzle);
		} else {
			sudoku::solve<9, false>(puzzle);
		}
	} else if (puzzle.size() == 36) {
		if (use_x) {
			sudoku::solve<6, true>(puzzle);
		} else {
			sudoku::solve<6, false>(puzzle);
		}
	} else {
		std::cout << "<puzzle> should be of length 36 or 81\n";
	}
}
