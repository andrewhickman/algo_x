#include "sudoku.h"

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
	}*/
	count = sudoku::solve_file(infile, outfile);
	double dt = double(clock() - t1) / CLOCKS_PER_SEC;
	if (count != 0) {
		std::cout << "Solved " << count << " sudoku puzzles in " << dt << " seconds\n";
		std::cout << "Average time: " << dt / count << '\n';
	}
}

int main() {
	sudoku_test();
}
