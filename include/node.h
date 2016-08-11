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
	HeadNode();                                // Head constructor
	HeadNode(HeadNode *row_);                  // Col constructor
	HeadNode(HeadNode *col_, size_t data_);    // Row constructor
	~HeadNode(); 

	size_t data;

	bool is_col() const;
	bool is_row() const;
	bool is_head() const;
};

#endif

