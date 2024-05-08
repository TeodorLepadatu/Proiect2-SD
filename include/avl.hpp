#ifndef AVL_HPP
#define AVL_HPP

#include <algorithm>
#include <limits>
#include <ostream>

class AVL {
	struct Node;

public:
	AVL();

	void Insert(const long long key);
	void Erase(const long long key);
	Node* Search(const long long key) const;
	Node* SearchSmaller(const long long key) const;
	Node* SearchGreater(const long long key) const;
	void PrintBetween(const long long key1, const long long key2, std::ostream &os);

	long long GetKey(Node *node) const;

private:
	struct Node {
		long long key;
		long long height;
		Node *left;
		Node *right;
	};

	Node *root_;

	Node* Insert(const long long key, Node *root);
	Node* Erase(const long long key, Node *root, Node *original_root);
	Node* Search(const long long key, Node *root) const;
	void PrintBetween(const long long key1, const long long key2, Node *root, std::ostream &os);

	Node* RotateLeft(Node *root);
	Node* RotateRight(Node *root);

	long long GetHeight(const Node *node) const;
	long long GetBalance(const Node *node) const;
	Node* GetSuccessor(const long long key, Node *root) const;
};

#endif //AVL_HPP