#include "sudoku.h"

#include <ctime>
#include <iostream>


void sudoku_test() {
	std::ifstream infile("test/top95.sudoku");
	std::ofstream outfile("test/top95.solutions");

	clock_t t1 = clock();
	int count = 0;
	for (std::string puzzle; std::getline(infile, puzzle);) {
		outfile << sudoku_solve(puzzle) << '\n';
		++count;
	}
	double dt = double(clock() - t1) / CLOCKS_PER_SEC;
	if (count != 0) {
		std::cout << "Solved " << count << " sudoku puzzles in " << dt << " seconds\n";
		std::cout << "Average time: " << dt / count << '\n';
	}
}

int main() {
	sudoku_test();
}
