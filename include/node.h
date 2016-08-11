#ifndef _node_h_
#define _node_h_

#include <cstdlib>
#include <iostream>

class HeadNode;

class Node {
public:
	Node(HeadNode *col_, HeadNode *row_);
	~Node();

	HeadNode *const col, *const row;
	Node *above, *below, *left, *right;

	void remove_from_col();
	void remove_from_row(); 
	void replace_in_col();
	void replace_in_row();

protected:
	Node(HeadNode *col_, HeadNode *row_,
	     Node *above_, Node *below_,
	     Node *left, Node *right);
};

class HeadNode : public Node {
public:
	HeadNode()
	: Node(this, this, this, this, this, this)
	, data(0) {
	}
	HeadNode(HeadNode *row_)
	: Node(this, row_, this, this, row_->left, row_)
	, data(0) {
	}
	HeadNode(HeadNode *col_, size_t data_) 
	: Node(col_, this, col_->above, col_, this, this)
	, data(data_) {
	}
	~HeadNode() {
		if (is_col()) {
			while (below != this) {
				delete below;
			}
		}
	}

	size_t data;

	bool is_col() const {
		return col == this;
	}
	bool is_row() const {
		return row == this;
	}
	bool is_head() const {
		return is_col() and is_row();
	}
};

#endif

