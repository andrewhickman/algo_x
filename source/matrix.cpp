#include "node.h"
#include "matrix.h"

SparseMatrix::SparseMatrix(size_t height, size_t width,
                           std::function<bool (size_t, size_t)> pred)
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
SparseMatrix::~SparseMatrix() {
/* Note all rows must be replaced before destruction */
	for (HeadNode* col : cols) {
		delete col;
	}
	delete head;
}
void SparseMatrix::remove_col(HeadNode* n) {
// Takes col header and removes its row */
	for (Node* it = n->below; it != n; it = it->below) {
		it->remove_from_row();
	}
	n->remove_from_row();
}
void SparseMatrix::remove_row(HeadNode* n) {
/* Takes row header and removes its row */
	for (Node* it = n->right; it != n; it = it->right) {
		it->remove_from_col();
	}
	n->remove_from_col();
}
void SparseMatrix::replace_col(HeadNode* n) {
/* Replaces previously removed col header */
	n->replace_in_row();
	for (Node* it = n->below; it != n; it = it->below) {
		it->replace_in_row();
	}
}
void SparseMatrix::replace_row(HeadNode* n) {
/* Replaces previously removed row header */
	n->replace_in_col();
	for (Node* it = n->right; it != n; it = it->right) {
		it->replace_in_col();
	}
}
HeadNode* SparseMatrix::min_col() const {
/* Return the column header with the smallest data entry */
	HeadNode* ret = head->right->col;
	size_t result, min = ret->data;
	for (Node* col = head->right; col != head; col = col->right) {
		result = col->col->data;
		if (result < min) {
			ret = col->col;
			min = result;
		}
	}
	return ret;
}
bool SparseMatrix::iterate(std::vector<HeadNode*>& solution) {
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
void SparseMatrix::iterate_all(std::vector<std::vector<HeadNode*>>& solutions) {
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



