#include "treap.hpp"

Treap::Treap() : root_(nullptr) {}

void Treap::Insert(const long long key)
{
	root_ = Insert(key, root_);
}

void Treap::Erase(const long long key)
{
	root_ = Erase(key, root_);
}

Treap::Node* Treap::Search(const long long key) const
{
	return Search(key, root_);
}

Treap::Node* Treap::SearchSmaller(const long long key) const
{
	if (root_ == nullptr) {
		return nullptr;
	}

	Node *result = nullptr;
	long long diff = std::numeric_limits<long long>::max();
	Node *current = root_;

	while (current != nullptr) {
		long long current_diff = key - current->key;

		if (current_diff >= 0 && current_diff <= diff) {
			diff = current_diff;
			result = current;
		}

		if (current->key > key) {
			current = current->left;
		} else {
			current = current->right;
		}
	}

	return result;
}

Treap::Node* Treap::SearchGreater(const long long key) const
{
	if (root_ == nullptr) {
		return nullptr;
	}

	Node *result = nullptr;
	long long diff = std::numeric_limits<long long>::max();
	Node *current = root_;

	while (current != nullptr) {
		long long current_diff = current->key - key;

		if (current_diff >= 0 && current_diff <= diff) {
			diff = current_diff;
			result = current;
		}

		if (current->key > key) {
			current = current->left;
		} else {
			current = current->right;
		}
	}

	return result;
}

void Treap::PrintBetween(const long long key1, const long long key2, std::ostream &os)
{
	PrintBetween(key1, key2, root_, os);
}

long long Treap::GetKey(Node* node) const
{
	if (node == nullptr) {
		return std::numeric_limits<long long>::min();
	}
	return node->key;
}

Treap::Node* Treap::Insert(const long long key, Node* root)
{
	if (root == nullptr) {
		return new Node{key, rand(), nullptr, nullptr};
	}

	if (key <= root->key) {
		root->left = Insert(key, root->left);

		if (root->left->priority > root->priority) {
			root = RotateRight(root);
		}
	} else {
		root->right = Insert(key, root->right);

		if (root->right->priority > root->priority) {
			root = RotateLeft(root);
		}
	}

	return root;
}

Treap::Node* Treap::Erase(const long long key, Node* root)
{
	if (root == nullptr) {
		return nullptr;
	}

	if (key < root->key) {
		root->left = Erase(key, root->left);
	} else if (key > root->key) {
		root->right = Erase(key, root->right);
	} else {
		if (root->left == nullptr) {
			Node *right_node = root->right;
			delete root;
			root = right_node;
		} else if (root->right == nullptr) {
			Node *left_node = root->left;
			delete root;
			root = left_node;
		} else {
			if (root->left->priority > root->right->priority) {
				root = RotateRight(root);
				root->right = Erase(key, root->right);
			} else {
				root = RotateLeft(root);
				root->left = Erase(key, root->left);
			}
		}
	}

	return root;
}

Treap::Node* Treap::Search(const long long key, Node* root) const
{
	if (root == nullptr) {
		return nullptr;
	}

	if (key == root->key) {
		return root;
	}

	if (key < root->key) {
		return Search(key, root->left);
	}

	return Search(key, root->right);
}

void Treap::PrintBetween(const long long key1, const long long key2, Node* root, std::ostream &os)
{
	if (root == nullptr) {
		return;
	}

	if (root->key > key1) {
		PrintBetween(key1, key2, root->left, os);
	}

	if (root->key >= key1 && root->key <= key2) {
		os << root->key << ' ';
	}

	if (root->key < key2) {
		PrintBetween(key1, key2, root->right, os);
	}
}


Treap::Node* Treap::RotateLeft(Node* root)
{
	Node *right_node = root->right;
	Node *left_of_right_node = right_node->left;

	right_node->left = root;
	root->right = left_of_right_node;

	return right_node;
}

Treap::Node* Treap::RotateRight(Node* root)
{
	Node *left_node = root->left;
	Node *right_of_left_node = left_node->right;

	left_node->right = root;
	root->left = right_of_left_node;

	return left_node;
}