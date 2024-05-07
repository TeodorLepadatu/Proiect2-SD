#include "avl.hpp"

AVL::AVL()
{
	root_ = nullptr;
}

void AVL::Insert(const long long key)
{
	root_ = Insert(key, root_);
}

void AVL::Erase(const long long key)
{
	root_ = Erase(key, root_, root_);
}

AVL::Node* AVL::Search(const long long key) const
{
	return Search(key, root_);
}

AVL::Node* AVL::SearchSmaller(const long long key) const
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

AVL::Node* AVL::SearchGreater(const long long key) const
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

void AVL::PrintBetween(const long long key1, const long long key2, std::ostream &os)
{
	PrintBetween(key1, key2, root_, os);
}

AVL::Node* AVL::Insert(const long long key, Node *root)
{
	if (root == nullptr) {
		return new Node{key, 1, nullptr, nullptr};
	}

	if (key < root->key) {
		root->left = Insert(key, root->left);
	} else if (root->key < key) {
		root->right = Insert(key, root->right);
	} else {
		return root;
	}

	root->height = 1 + std::max(GetHeight(root->left), GetHeight(root->right));
	const long long balance = GetBalance(root);

	// Left Left
	if (balance > 1 && key < root->left->key) {
		return RotateRight(root);
	}

	// Right Right
	if (balance < -1 && root->right->key < key) {
		return RotateLeft(root);
	}

	// Left Right
	if (balance > 1 && root->left->key < key) {
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}

	// Right Left
	if (balance < -1 && key < root->right->key) {
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}

	return root;
}

AVL::Node* AVL::Erase(const long long key, Node *root, Node *original_root)
{
	if (root == nullptr) {
		return root;
	}

	if (key < root->key) {
		root->left = Erase(key, root->left, original_root);
	} else if (key > root->key) {
		root->right = Erase(key, root->right, original_root);
	} else {
		if (root->left == nullptr || root->right == nullptr) {
			// One child or no child
			Node *temp = (root->left != nullptr) ? root->left : root->right;

			if (temp == nullptr) {
				// No child
				temp = root;
				root = nullptr;
			} else {
				// One child
				*root = *temp;
			}

			free(temp);
		} else {
			// Two children
			Node *temp = GetSuccessor(root->key, original_root);

			root->key = temp->key;
			root->right = Erase(temp->key, root->right, original_root);

			free(temp);
		}
	}

	// If there is only one node left
	if (root == nullptr) {
		return root;
	}

	root->height = 1 + std::max(GetHeight(root->left), GetHeight(root->right));
	const long long balance = GetBalance(root);

	// Left Left
	if (balance > 1 && GetBalance(root->left) >= 0) {
		return RotateRight(root);
	}

	// Right Right
	if (balance < -1 && GetBalance(root->right) <= 0) {
		return RotateLeft(root);
	}

	// Left Right
	if (balance > 1 && GetBalance(root->left) < 0) {
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}

	// Right Left
	if (balance < -1 && GetBalance(root->right) > 0) {
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}

	return root;
}

AVL::Node* AVL::Search(const long long key, Node *root) const
{
	while (root != nullptr && root->key != key) {
		if (key > root->key) {
			root = root->right;
		} else {
			root = root->left;
		}
	}

	if (root != nullptr && root->key == key) {
		return root;
	} else {
		return nullptr;
	}
}

void AVL::PrintBetween(const long long key1, const long long key2, Node *root, std::ostream &os)
{
	if (root == nullptr) {
		return;
	}

	if (key1 < root->key) {
		PrintBetween(key1, key2, root->left, os);
	}

	if (key1 <= root->key && root->key <= key2) {
		os << root->key << " ";
	}

	if (root->key < key2) {
		PrintBetween(key1, key2, root->right, os);
	}
}

AVL::Node* AVL::RotateLeft(Node *root)
{
	Node *right_node = root->right;
	Node *left_of_right_node = right_node->left;

	right_node->left = root;
	root->right = left_of_right_node;

	root->height = 1 + std::max(GetHeight(root->left), GetHeight(root->right));
	right_node->height = 1 + std::max(GetHeight(right_node->left), GetHeight(right_node->right));

	return right_node;
}

AVL::Node* AVL::RotateRight(Node *root)
{
	Node *left_node = root->left;
	Node *right_of_left_node = left_node->right;

	left_node->right = root;
	root->left = right_of_left_node;

	root->height = 1 + std::max(GetHeight(root->left), GetHeight(root->right));
	left_node->height = 1 + std::max(GetHeight(left_node->left), GetHeight(left_node->right));

	return left_node;
}
 

long long AVL::GetHeight(const Node *node) const
{
	return node == nullptr ? 0 : node->height;
}

long long AVL::GetBalance(const Node *node) const
{
	return node == nullptr ? 0 : GetHeight(node->left) - GetHeight(node->right);
}

AVL::Node* AVL::GetSuccessor(const long long key, Node *root) const
{
	Node *successor = Search(key, root);

	if (successor->right != nullptr) {
		successor = successor->right;

		while (successor->left != nullptr) {
			successor = successor->left;
		}

		return successor;
	} else {
		Node *ancestor = root;
		Node *successor = nullptr;

		while (ancestor != successor) {
			if (key < ancestor->key) {
				successor = ancestor;
				ancestor = ancestor->left;
			} else {
				ancestor = ancestor->right;
			}
		}

		return successor;
	}
}