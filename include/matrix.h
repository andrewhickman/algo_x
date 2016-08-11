#ifndef _matrix_h_
#define _matrix_h_

#include "node.h"

#include <cstdlib>
#include <functional>
#include <set>
#include <vector>

struct SparseMatrix {
	SparseMatrix(size_t height, size_t width,
	             std::function<bool (size_t, size_t)> pred);
	~SparseMatrix();

	HeadNode* head;
	std::vector<HeadNode*> cols;
	std::vector<HeadNode*> rows;

	void remove_col(HeadNode* n);
	void remove_row(HeadNode* n);
	void replace_col(HeadNode* n);
	void replace_row(HeadNode* n);
	HeadNode* min_col() const;

	bool iterate(std::vector<HeadNode*>& solution);
	void iterate_all(std::vector<std::vector<HeadNode*>>& solutions);

	std::vector<HeadNode*> solve() {
/* Find a single solution to the exact cover problem. */
		std::vector<HeadNode*> ret;
		iterate(ret);
		return ret;
	}

	std::vector<std::vector<HeadNode*>> solve_all() {
/* Find all solutions to the exact cover problem. */
		std::vector<std::vector<HeadNode*>> ret;
		iterate_all(ret);
		return ret;
	}

};

#endif
