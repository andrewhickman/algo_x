#include "sudoku.h"

#include <ctime>
#include <fstream>
#include <iostream>

int main() {
	std::ifstream infile("tests/sudoku/top2365.sudoku");
	std::ofstream outfile("tests/sudoku/top2365.solutions");

	clock_t t1 = clock();
	int count = sudoku::solve_file<9, false>(infile, outfile);
	double dt = double(clock() - t1) / CLOCKS_PER_SEC;
	if (count != 0) {
		std::cout << "Solved " << count << " sudoku puzzles in " << dt << " seconds\n";
		std::cout << "Average time: " << dt / count << " seconds\n";
	}
}

