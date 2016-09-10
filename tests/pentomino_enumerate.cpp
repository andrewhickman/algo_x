#include "pentomino.h"

#include <iostream>
#include <string>

using namespace pentomino;

std::string format_solution(std::vector<HeadNode*> sol) {
    std::string ret(64, ' ');
    for (HeadNode *n : sol) {
        int data = n->data;
        int y = data % 8;
        int x = (data /= 8) % 8;
        const Pentomino& pentomino = fixed_pentominoes()[data /= 8];
        for (Position p : pentomino.squares) {
            p.x += x;
            p.y += y;
            ret[p.x + 8 * p.y] = pentomino.name;
        }
    }
    return ret;
}

int main() {
    SparseMatrix m = create_matrix();
    for (const auto& sol : m.solve_all()) {
        std::cout << format_solution(sol) << '\n';
    }
}
