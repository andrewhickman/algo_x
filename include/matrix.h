#include "node.h"

#include <cstdlib>
#include <iostream>
#include <functional>
#include <limits>
#include <set>
#include <vector>

struct SparseMatrix {
	SparseMatrix(size_t height, size_t width, std::function<bool (size_t, size_t)> pred)
	: head(new HeadNode())
	, cols(width)
	, rows(height) {
		// Setup col headers
		for (size_t j = 0; j < width; ++j) {
			cols[j] = new ColNode(head);
		}
		// Fill in 1s
		for (size_t i = 0; i < height; ++i) {
			rows[i] = new RowNode(head, i);
			for (size_t j = 0; j < width; ++j) {
				if (pred(j, i)) {
					new Node(rows[i], cols[j]);
				}
			}
			// Empty rows are removed
			if (rows[i]->right == rows[i]) {
				delete rows[i];
			}
		}
	}

	~SparseMatrix() {
	// Note all rows and cols must be replaced before destruction to avoid memory leaks
		delete head;
	}

	void remove_row(RowNode* n) {
		for (Node* it = n->right; it != n; it = it->right) {
			it->remove_from_col();
		}
		n->remove_from_col();
	}

	void remove_col(ColNode* n) {
		for (Node* it = n->below; it != n; it = it->below) {
			it->remove_from_row();
		}
		n->remove_from_row();
	}

	void replace_row(RowNode* n) {
		n->replace_in_col();
		for (Node* it = n->right; it != n; it = it->right) {
			it->replace_in_col();
		}
	}

	void replace_col(ColNode* n) {
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
		//c = min_col();
		c = head->right->col;
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
			for (auto it = removed_cols.begin(); it != removed_cols.end(); ++it) {
				remove_col(*it);
			}
			for (auto it = removed_rows.begin(); it != removed_rows.end(); ++it) {
				remove_row(*it);
			}
			//std::cout << removed_cols.size() << ' ' << removed_rows.size() << '\n';
			// Recursively apply the algorithm to the reduced SparseMatrix
			result = iterate(solution);
			// Replace the removed rows and columns
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
	std::vector<ColNode*> cols;
	std::vector<RowNode*> rows;
};
