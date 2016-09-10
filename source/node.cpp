#include "node.h"


Node::Node(HeadNode *col_, HeadNode *row_)
: col(col_)
, row(row_)
, above(col->above)
, below(col)
, left(row->left)
, right(row) {
	replace_in_col();
	replace_in_row();
}

Node::Node(HeadNode *col_, HeadNode *row_,
     Node *above_, Node *below_,
     Node *left_, Node *right_) 
: col(col_)
, row(row_)
, above(above_)
, below(below_)
, left(left_)
, right(right_) {
	replace_in_col();
	replace_in_row();
}

Node::~Node() {
	remove_from_col();
	remove_from_row();
}

void Node::remove_from_col() {
	above->below = below;
	below->above = above;
	--col->data;
}

void Node::remove_from_row() { 
	left->right = right;
	right->left = left;
}

void Node::replace_in_col() {
	above->below = this;
	below->above = this;
	++col->data;
}

void Node::replace_in_row() {
	left->right = this;
	right->left = this;
} 

HeadNode::HeadNode()
: Node(this, this, this, this, this, this)
, data(0) {
}

HeadNode::HeadNode(HeadNode *row_)
: Node(this, row_, this, this, row_->left, row_)
, data(0) {
}

HeadNode::HeadNode(HeadNode *col_, size_t data_) 
: Node(col_, this, col_->above, col_, this, this)
, data(data_) {
}

HeadNode::~HeadNode() {
    if (is_col()) {
        while (below != this) {
            delete below;
        }
    } 
}

bool HeadNode::is_col() const {
	return col == this;
}
bool HeadNode::is_row() const {
	return row == this;
}
bool HeadNode::is_head() const {
	return is_col() and is_row();
}

