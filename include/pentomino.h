#ifndef _pentomino_h_
#define _pentomino_h_

#include "matrix.h"

/*  Find all tilings of the chessboard with centre removed by the 12
 *  pentominoes
 */

namespace pentomino {

namespace constraints {

struct Pentomino {
    bool data[5][5];

    bool is_covered(int x, int y, int rotation, bool is_reflected) const {
    /*  <rotation> defines the 4 possible pentomino rotations in some order.
     *  Together with <is_reflected> we get all orientations
     */
        if (x < 0 or x > 4 or y < 0 or y > 4) {
            return false;
        }
        if (is_reflected) {
            x = 4 - x;
        }
        switch (rotation) {
            case 0:
                return data[x][y];
            case 1:
                return data[y][4 - x];
            case 2:
                return data[4 - x][4 - y];
            case 3:
                return data[4 - y][x];
        }
        return false;    // unused
    }
};

const Pentomino pentominoes[12] = {
    {{{0,1,1,0,0},{1,1,0,0,0},{0,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // F
    {{{1,1,1,1,1},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // I
    {{{1,1,1,1,0},{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // L
    {{{1,1,1,0,0},{0,0,1,1,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // N
    {{{1,1,1,0,0},{1,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // P
    {{{1,1,1,0,0},{0,1,0,0,0},{0,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // T
    {{{1,1,1,0,0},{1,0,1,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // U
    {{{1,1,1,0,0},{1,0,0,0,0},{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // V
    {{{0,1,1,0,0},{1,1,0,0,0},{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // W
    {{{0,1,0,0,0},{1,1,1,0,0},{0,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // X
    {{{1,1,1,1,0},{0,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // Y
    {{{1,1,0,0,0},{0,1,0,0,0},{0,1,1,0,0},{0,0,0,0,0},{0,0,0,0,0}}},    // Z
};

inline bool matrix(int j, int i) {
    /*  Return true if the unique pentomino defined by i covers the tile
     *  defined by j
     */

    if (j == 36 or j == 37 or j == 44 or j == 45) {
        return true;
    }

    int type = i % 12;
    int rotation = (i /= 12) % 4;
    int reflect = (i /= 4) % 2;
    int pos_x = (i /= 2) % 12;    // the position of the bottom right of the
    int pos_y = (i /= 12) % 12;   // pentomino's 5x5 bounding box

    pos_x = (j % 8) - (pos_x - 4);
    pos_y += ((j /= 8) % 8) - (pos_y - 4);
    
    return pentominoes[type].is_covered(pos_x, pos_y, rotation, reflect);
}

} // namespace constraints

int count() {
    /* Count number of solutions */
	int height = 12 * 4 * 2 * 12 * 12;
	int width = 8 * 8;
	SparseMatrix M(height, width, constraints::matrix);
	auto solutions = M.solve();
    return solutions.size();
}

} // namespace pentomino 

#endif

