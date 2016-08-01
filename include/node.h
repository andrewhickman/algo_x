#include <cstdlib>

struct HeadNode;
struct RowNode;
struct ColNode;

struct Node {
public:
	Node(RowNode* row_, ColNode* col_);

	void remove_from_row();
	void remove_from_col();
	void replace_in_row();
	void replace_in_col();

	RowNode* row;
	ColNode* col;
	Node* left, * right, * above, * below;

protected:
	Node()
	: row(nullptr)
	, col(nullptr)
	, left(this)
	, right(this)
	, above(this)
	, below(this) {
	}
	Node(RowNode* row_, ColNode* col_, Node* l, Node* r, Node* a, Node* b)
	: row(row_), col(col_), left(l), right(r), above(a), below(b) {
		replace_in_row();
		replace_in_col();
	}
public:
	virtual ~Node() {
		// Clean up pointers to this node
		remove_from_row();
		remove_from_col();
	}
};

struct RowNode : virtual public Node {
	RowNode(HeadNode* head, size_t num_);

	size_t num;

protected:
	RowNode() = default;
	~RowNode() {
		while (right != this) {
			delete right;
		}
	}
};

struct ColNode : virtual public Node {
	ColNode(HeadNode* head);

	size_t size;

protected:
	ColNode() = default;
	~ColNode() {
		while (below != this) {
			delete below;
		}
	}
};

struct HeadNode : public RowNode, public ColNode {
	HeadNode() {
		row = this;
		col = this;
	}
};

RowNode::RowNode(HeadNode* head, size_t num_)
: Node(this, head, this, this, head->above, head)
, num(num_) {
}

ColNode::ColNode(HeadNode* head)
: Node(head, this, head->left, head, this, this)
, size(0) {
}

Node::Node(RowNode* row_, ColNode* col_)
: row(row_), col(col_)    // Element is added at end of row and col
, left(row->left)
, right(row)
, above(col->above)
, below(col) {
	replace_in_row();
	replace_in_col();
}

void Node::remove_from_row() {
	left->right = right;
	right->left = left;
}

void Node::remove_from_col() {
	above->below = below;
	below->above = above;
	--col->size;
}

void Node::replace_in_row() {
	left->right = this;
	right->left = this;
}

void Node::replace_in_col() {
	above->below = this;
	below->above = this;
	++col->size;
}
