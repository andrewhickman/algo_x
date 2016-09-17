#ifndef _matrix_h_
#define _matrix_h_

#include "node.h"

#include <cstdlib>
#include <functional>
#include <memory>
#include <set>
#include <vector>

struct SparseMatrix {
    SparseMatrix(size_t height, size_t width,
                 std::function<bool (size_t, size_t)> pred);

    SparseMatrix(size_t width);
    void create_row(size_t data, const std::set<int>& elems);

    std::unique_ptr<HeadNode> head;
    std::vector<std::unique_ptr<HeadNode>> cols;
    std::vector<HeadNode*> rows;

    void remove_col_and_rows(HeadNode*);
    void replace_col_and_rows(HeadNode*);
    void remove_row(Node*);
    void replace_row(Node*);
    HeadNode *min_col() const;

    bool iterate(std::vector<HeadNode*>& solution);
    void iterate_all(std::vector<std::vector<HeadNode*>>& solutions);

    std::vector<HeadNode*> solve();
    std::vector<std::vector<HeadNode*>> solve_all();
};

#endif

