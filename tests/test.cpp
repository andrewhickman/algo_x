#include "sudoku.h"

#include <iostream>


int main() {
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 6; ++i) {
			int num = sudoku::constraints::cross<6>(j, i, 0);
			std::cout << num << (num < 10 ? "  " : " ");
		}
		std::cout << '\n';
	}
}
