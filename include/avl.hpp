#ifndef AVL_HPP
#define AVL_HPP

#include <algorithm>
#include <limits>
#include <ostream>

class AVL {
public:
	AVL();

	void Insert(const long long key);
	void Erase(const long long key);
	bool Search(const long long key) const;
	std::pair<bool, long long> SearchSmaller(const long long key) const;
	std::pair<bool, long long> SearchGreater(const long long key) const;
	void PrintBetween(const long long key1, const long long key2, std::ostream &os);

private:
	struct Node {
		long long key;
		long long height;
		Node *left;
		Node *right;
	};

	Node *root_;

	Node* InsertNode(const long long key, Node *root);
	Node* EraseNode(const long long key, Node *root, Node *original_root);
	Node* SearchNode(const long long key, Node *root) const;
	void PrintBetweenRecursive(const long long key1, const long long key2, Node *root, std::ostream &os);

	Node* RotateLeft(Node *root);
	Node* RotateRight(Node *root);

	long long GetHeight(const Node *node) const;
	long long GetBalance(const Node *node) const;
	Node* GetSuccessor(const long long key, Node *root) const;
};

#endif //AVL_HPP