#include "sudoku.h"
#include "sudoku_x.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

void sudoku_test() {
	std::ifstream infile("test/sudoku/top2365.sudoku");
	std::ofstream outfile("test/sudoku/top2365.solutions");

	clock_t t1 = clock();
	int count = 0;
	/*
	for (std::string puzzle; std::getline(infile, puzzle);) {
		outfile << sudoku::solve(puzzle) << '\n';
		++count;
	}
	*/
	count += sudoku::solve_file(infile, outfile);
	double dt = double(clock() - t1) / CLOCKS_PER_SEC;
	if (count != 0) {
		std::cout << "Solved " << count << " sudoku puzzles in " << dt << " seconds\n";
		std::cout << "Average time: " << dt / count << " seconds\n";
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			std::string puzzle = argv[i];
			if (puzzle.size() == 81) {
				sudoku::solve(puzzle);
			} else if (puzzle.size() == 36) {
				sudoku_x::solve(puzzle);
			}
		}
	} else {
		sudoku_test();
	}
}
