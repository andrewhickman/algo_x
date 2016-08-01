#include "node.h"

#include <cassert>
#include <cstdlib>
#include <functional>
#include <limits>
#include <set>
#include <vector>

struct SparseMatrix {
	SparseMatrix(size_t height, size_t width, std::function<bool (size_t, size_t)> pred)
	: head(new HeadNode()) {
		std::vector<ColNode*> cols(width);
		RowNode* row;
		// Setup col headers
		for (size_t j = 0; j < width; ++j) {
			cols[j] = new ColNode(head);
		}
		// Fill in 1s
		for (size_t i = 0; i < height; ++i) {
			row = new RowNode(head, i);
			for (size_t j = 0; j < width; ++j) {
				if (pred(j, i)) {
					new Node(row, cols[j]);
				}
			}
			// Empty rows are removed
			if (row->right == row) {
				//delete row;
			}
		}
	}

	~SparseMatrix() {
	// Note all rows and cols must be replaced before destruction to avoid memory leaks
		delete head;
	}

	void remove_row(Node* n) {
		for (Node* it = n->right; it != n; it = it->right) {
			it->remove_from_col();
		}
		n->remove_from_col();
	}

	void remove_col(Node* n) {
		for (Node* it = n->below; it != n; it = it->below) {
			it->remove_from_row();
		}
		n->remove_from_row();
	}

	void replace_row(Node* n) {
		n->replace_in_col();
		for (Node* it = n->right; it != n; it = it->right) {
			it->replace_in_col();
		}
	}

	void replace_col(Node* n) {
		n->replace_in_row();
		for (Node* it = n->below; it != n; it = it->below) {
			it->replace_in_row();
		}
	}

	ColNode* min_col() const {
		ColNode* ret = head->right->col;
		size_t result, min = ret->size;
		for (ColNode* col = head->right->col; col != head; col = col->right->col) {
			result = col->col->size;
			if (result < min) {
				ret = col;
				min = result;
			}
		}
		return ret;
	}

	bool iterate(std::vector<RowNode*>& solution) {
		if (head->right == head) {
			return true;
		}
		RowNode* r, * i;
		ColNode* c, * j;
		std::set<RowNode*> removed_rows;
		std::set<ColNode*> removed_cols;
		bool result;
		c = min_col();
		for (Node* trial = c->below; trial != c; trial = trial->below) {
			r = trial->row;
			for (Node* obj = r->right; obj != r; obj = obj->right) {
				j = obj->col;
				for (Node* set = j->below; set != j; set = set->below) {
					i = set->row;
					removed_rows.insert(i);
				}
				removed_cols.insert(j);
			}
			// Remove rows and columns containing elements we have already
			for (Node* n : removed_cols) {
				remove_col(n);
			}
			for (Node* n : removed_rows) {
				remove_row(n);
			}
			// Recursively apply the algorithm to the reduced SparseMatrix
			result = iterate(solution);
			// Replaced the removed rows and columns
			for (auto it = removed_rows.rbegin(); it != removed_rows.rend(); ++it) {
				replace_row(*it);
			}
			removed_rows.clear();
			for (auto it = removed_cols.rbegin(); it != removed_cols.rend(); ++it) {
				replace_col(*it);
			}
			removed_cols.clear();
			if (result) {
				solution.push_back(r);
				return true;
			}
		}
		return false;
	}

	std::vector<RowNode*> solve() {
		std::vector<RowNode*> ret;
		iterate(ret);
		return ret;
	}

	HeadNode* head;
};
