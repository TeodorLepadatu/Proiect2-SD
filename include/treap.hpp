#ifndef TREAP_HPP
#define TREAP_HPP

#include <algorithm>
#include <limits>
#include <ostream>

class Treap{
	struct Node;

public:
	Treap();

	void Insert(const long long key);
	void Erase(const long long key);
	Node* Search(const long long key) const;
	Node* SearchSmaller(const long long key) const;
	Node* SearchGreater(const long long key) const;
	void PrintBetween(const long long key1, const long long key2, std::ostream &os);

private:
	Node* Insert(const long long key, Node* root);
	Node* Erase(const long long key, Node* root);
	Node* Search(const long long key, Node* root) const;
	void PrintBetween(const long long key1, const long long key2, Node* root, std::ostream &os);

	Node* RotateLeft(Node* root);
	Node* RotateRight(Node* root);

	struct Node {
		long long key;
		long long priority;
		Node* left;
		Node* right;
	};

	Node* root_;
};

#endif //TREAP_HPP