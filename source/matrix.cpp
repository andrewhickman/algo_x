#include "matrix.h"

using namespace std;

SparseMatrix::SparseMatrix(size_t width)
: head(new HeadNode())
, cols(width)
, rows() {
    /* Create matrix with no rows */
    for (size_t j = 0; j < width; ++j) {
        cols[j] = make_unique<HeadNode>(head.get());
    }
}

SparseMatrix::SparseMatrix(size_t height, size_t width,
                           function<bool (size_t, size_t)> pred)
: head(new HeadNode())
, cols(width)
, rows(height) {
    // Setup col headers
    for (size_t j = 0; j < width; ++j) {
        cols[j] = make_unique<HeadNode>(head.get());
    }
    // Fill in rows 
    for (size_t i = 0; i < height; ++i) {
        rows[i] = new HeadNode(head.get(), i);
        for (size_t j = 0; j < width; ++j) {
            if (pred(j, i)) {
                new Node(cols[j].get(), rows[i]);
            }
        }
    }
}

void SparseMatrix::create_row(size_t data, const std::set<int>& col_nums) {
    /* Add new row to matrix with given columns. */
    HeadNode *row = new HeadNode(head.get(), data);
    rows.push_back(row);
    for (int col_num : col_nums) {
        new Node(cols[col_num].get(), row);
    }
}

void SparseMatrix::remove_col_and_rows(HeadNode *col) {
/* Remove a column and all rows it has a 1 in */
    col->remove_from_row();
    for (Node *i = col->below; i != col; i = i->below) {
        for (Node *j = i->right; j != i; j = j->right) {
             j->remove_from_col();
        }
    }
}

void SparseMatrix::replace_col_and_rows(HeadNode *col) {
/* Replace a column and all rows it has a 1 in */
    for (Node *i = col->above; i != col; i = i->above) {
        for (Node *j = i->left; j != i; j = j->left) {
           j->replace_in_col();
        }
    }
    col->replace_in_row();
}

void SparseMatrix::remove_row(Node *i) {
    for (Node *j = i->right; j != i; j = j->right) {
       j->remove_from_col();
    }
}
        
void SparseMatrix::replace_row(Node *i) {
    for (Node *j = i->left; j != i; j = j->left) {
       j->replace_in_col();
    }
}

HeadNode* SparseMatrix::min_col() const {
/* Return the column header with the smallest data entry */
    HeadNode* ret = head->right->col;
    size_t result, min = ret->data;
    for (Node* col = head->right; col != head.get(); col = col->right) {
        result = col->col->data;
        if (result < min) {
            ret = col->col;
            min = result;
        }
    }
    return ret;
}

bool SparseMatrix::iterate(vector<HeadNode*>& solution) {
    if (head->right == head.get()) {
        return true;
    }
    bool result = false;
    HeadNode* c = min_col();
    remove_col_and_rows(c);
    for (Node *r = c->below; r != c; r = r->below) {
        for (Node *j = r->row->right; j != r->row; j = j->right) {
            if (j != r) {
                remove_col_and_rows(j->col);
            }
        }
        result = iterate(solution);
        for (Node *j = r->row->left; j != r->row; j = j->left) {
            if (j != r) {
                replace_col_and_rows(j->col);
            }
        }
        if (result) {
            solution.push_back(r->row);
            break;
        }
    }
    replace_col_and_rows(c);
    return result;
}

void SparseMatrix::iterate_all(vector<std::vector<HeadNode*>>& solutions) {
    static vector<HeadNode*> current_solution;
    if (head->right == head.get()) {
        solutions.push_back(current_solution);
        return;
    }
    HeadNode* c = min_col();
    remove_col_and_rows(c);
    for (Node *r = c->below; r != c; r = r->below) {
        for (Node *j = r->row->right; j != r->row; j = j->right) {
            if (j != r) {
                remove_col_and_rows(j->col);
            }
        }
        current_solution.push_back(r->row);
        iterate_all(solutions);
        current_solution.pop_back();
        for (Node *j = r->row->left; j != r->row; j = j->left) {
            if (j != r) {
                replace_col_and_rows(j->col);
            }
        }
    }
    replace_col_and_rows(c);
}

vector<HeadNode*> SparseMatrix::solve() {
/* Find a single solution to the exact cover problem. */
    std::vector<HeadNode*> ret;
    iterate(ret);
    return ret;
}

vector<vector<HeadNode*>> SparseMatrix::solve_all() {
/* Find all solutions to the exact cover problem. */
    vector<std::vector<HeadNode*>> ret;
    iterate_all(ret);
    return ret;
}
