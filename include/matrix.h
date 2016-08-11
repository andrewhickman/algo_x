#ifndef _matrix_h_
#define _matrix_h_

#include "node.h"

#include <cstdlib>
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
			cols[j] = new HeadNode(head);
		}
		// Fill in rows 
		for (size_t i = 0; i < height; ++i) {
			rows[i] = new HeadNode(head, i);
			for (size_t j = 0; j < width; ++j) {
				if (pred(j, i)) {
					new Node(cols[j], rows[i]);
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
		for (HeadNode* col : cols) {
			delete col;
		}
		delete head;
	}

	void remove_row(HeadNode* n) {
		for (Node* it = n->right; it != n; it = it->right) {
			it->remove_from_col();
		}
		n->remove_from_col();
	}

	void remove_col(HeadNode* n) {
		for (Node* it = n->below; it != n; it = it->below) {
			it->remove_from_row();
		}
		n->remove_from_row();
	}

	void replace_row(HeadNode* n) {
		n->replace_in_col();
		for (Node* it = n->right; it != n; it = it->right) {
			it->replace_in_col();
		}
	}

	void replace_col(HeadNode* n) {
		n->replace_in_row();
		for (Node* it = n->below; it != n; it = it->below) {
			it->replace_in_row();
		}
	}

	HeadNode* min_col() const {
		HeadNode* ret = head->right->col;
		size_t result, min = ret->data;
		for (HeadNode* col = head->right->col; col != head; col = col->right->col) {
			result = col->col->data;
			if (result < min) {
				ret = col;
				min = result;
			}
		}
		return ret;
	}

	bool iterate(std::vector<HeadNode*>& solution) {
	/* Find a solution to the exact cover problem. Halts after solution is found */
		if (head->right == head) {
			return true;
		}
		HeadNode* r, * i;
		HeadNode* c, * j;
		std::set<HeadNode*> removed_rows;
		std::set<HeadNode*> removed_cols;
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
			for (auto it = removed_cols.begin(); it != removed_cols.end(); ++it) {
				remove_col(*it);
			}
			for (auto it = removed_rows.begin(); it != removed_rows.end(); ++it) {
				remove_row(*it);
			}
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

	void iterate_all(std::vector<std::vector<HeadNode*>>& solutions) {
	/* Find all solutions to the exact cover problem. */
		static std::vector<HeadNode*> current_solution;
		if (head->right == head) {
			solutions.push_back(current_solution);
			return;
		}
		HeadNode* r, * i;
		HeadNode* c, * j;
		std::set<HeadNode*> removed_rows;
		std::set<HeadNode*> removed_cols;
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
			for (auto it = removed_cols.begin(); it != removed_cols.end(); ++it) {
				remove_col(*it);
			}
			for (auto it = removed_rows.begin(); it != removed_rows.end(); ++it) {
				remove_row(*it);
			}
			// Recursively apply the algorithm to the reduced SparseMatrix
			current_solution.push_back(r);
			iterate_all(solutions);
			current_solution.pop_back();
			// Replace the removed rows and columns
			for (auto it = removed_rows.rbegin(); it != removed_rows.rend(); ++it) {
				replace_row(*it);
			}
			removed_rows.clear();
			for (auto it = removed_cols.rbegin(); it != removed_cols.rend(); ++it) {
				replace_col(*it);
			}
			removed_cols.clear();
		}
	}

	std::vector<HeadNode*> solve() {
		std::vector<HeadNode*> ret;
		iterate(ret);
		return ret;
	}

	std::vector<std::vector<HeadNode*>> solve_all() {
		std::vector<std::vector<HeadNode*>> ret;
		iterate_all(ret);
		return ret;
	}

	HeadNode* head;
	std::vector<HeadNode*> cols;
	std::vector<HeadNode*> rows;
};

#endif
