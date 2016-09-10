#ifndef _pentomino_h_
#define _pentomino_h_

#include "matrix.h"

#include <cassert>

/*  Find all tilings of the chessboard with centre removed by the 12
 *  pentominoes
 */

namespace pentomino {

struct Position {
    int x, y;

    int get_col_num() const {
        if (x < 0 or 8 <= x or y < 0 or 8 <= y)
            return 0;
        int ret = x + 8 * y;
        if (ret < 27)
            return 12 + ret;
        if (ret < 29)
            return 0;
        if (ret < 35)
            return 10 + ret;
        if (ret < 37)
            return 0;
        return 8 + ret;
    }
};

struct Pentomino {
    Position squares[5];
    int id;
    char name;
};

inline Position apply_transform(Position p, int rotation, bool reflect) {
    if (reflect) {
        p.x = -p.x;
    }
    switch (rotation) {
      default:
        return {p.x, p.y};
      case 1:
        return {p.y, -p.x};
      case 2:
        return {-p.x, -p.y};
      case 3:
        return {-p.y, p.x};
    }
}

inline Pentomino transform_pentomino(const Pentomino& p, int rotation, bool reflect) {
    return {
        {
            apply_transform(p.squares[0], rotation, reflect),
            apply_transform(p.squares[1], rotation, reflect),
            apply_transform(p.squares[2], rotation, reflect),
            apply_transform(p.squares[3], rotation, reflect),
            apply_transform(p.squares[4], rotation, reflect),
        },
        p.id,
        p.name,
    };
}


const Pentomino base_pentominoes[12] = {
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}, {1, 2}}, 0, 'F'},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}, 1, 'I'},
    {{{0, 0}, {1, 0}, {1, 1}, {1, 2}, {1, 3}}, 2, 'L'},
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}, {3, 1}}, 3, 'N'},
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}, {2, 1}}, 4, 'P'},
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}, {1, 2}}, 5, 'T'},
    {{{0, 0}, {1, 0}, {1, 1}, {0, 2}, {1, 2}}, 6, 'U'},
    {{{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}}, 7, 'V'},
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2}}, 8, 'W'},
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}, {1,-1}}, 9, 'X'},
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {1, 1}}, 10, 'Y'},
    {{{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2}}, 11, 'Z'},
};

const std::vector<Pentomino>& fixed_pentominoes() {
    static auto ret = []{
        std::vector<Pentomino> v;
        for (const auto& p : base_pentominoes) {
            switch (p.name) {
                case 'F':
                case 'L':
                case 'N':
                case 'P':
                case 'Y':    // All rotations and reflections
                    v.push_back(transform_pentomino(p, 1, true));
                    v.push_back(transform_pentomino(p, 2, true));
                    v.push_back(transform_pentomino(p, 3, true));
                    v.push_back(transform_pentomino(p, 4, true));
                case 'T':
                case 'U':
                case 'V':
                case 'W':    // All rotations
                    v.push_back(transform_pentomino(p, 0, false));
                    v.push_back(transform_pentomino(p, 1, false));
                    v.push_back(transform_pentomino(p, 2, false));
                    v.push_back(transform_pentomino(p, 3, false));
                    break;
                case 'Z':    // Rotate by 90 degrees and reflection
                    v.push_back(transform_pentomino(p, 0, true));
                    v.push_back(transform_pentomino(p, 1, true));
                case 'I':     // Rotate by 90 degrees
                    v.push_back(transform_pentomino(p, 1, false));
                case 'X':     // No transforms
                    v.push_back(transform_pentomino(p, 0, false));
            }
        }
        assert(v.size() == 63);
        return v;
    }();
    return ret;
}

SparseMatrix create_matrix() {
    SparseMatrix ret(72);
    size_t row_id = 0;
    for (const Pentomino& p : fixed_pentominoes()) {
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                std::set<int> cols;
                for (Position pos : p.squares) {
                    pos.x += x;
                    pos.y += y;
                    int col = pos.get_col_num();
                    if (col == 0) {
                        goto next;
                    }
                    cols.insert(col);
                }
                cols.insert(p.id);
                ret.create_row(row_id, cols);
            next:
                ++row_id;
            }
        }
    }
    assert(ret.rows.size() == 1568);
    return ret;
}



} // namespace pentomino

#endif

