#include "btree.hpp"

BTree::BTree(const size_t min_degree)
	: min_degree_(min_degree)
{
	root_ = new BTreeNode(min_degree_, true);
}

void BTree::Insert(const long long key)
{
	if (root_ == nullptr) {
		root_ = new BTreeNode(min_degree_, true);
		root_->keys_[0] = key;
		root_->key_count_ = 1;
		return;
	}

	if (root_->key_count_ == 2 * min_degree_ - 1) {
		BTreeNode *new_root = new BTreeNode(min_degree_, false);
		new_root->children_[0] = root_;
		new_root->SplitChild(0, root_);

		size_t i = 0;
		if (new_root->keys_[0] < key) {
			++i;
		}
		new_root->children_[i]->InsertNonFull(key);

		root_ = new_root;
	} else {
		root_->InsertNonFull(key);
	}
}

void BTree::Erase(const long long key)
{
	if (root_ == nullptr) {
		return;
	}

	root_->Erase(key);

	if (root_->key_count_ == 0) {
		BTreeNode *tmp = root_;
		if (root_->leaf_ == true) {
			root_ = nullptr;
		} else {
			root_ = root_->children_[0];
		}
		delete tmp;
	}
}

BTreeNode* BTree::Search(const long long key) const
{
	return root_ == nullptr ? nullptr : root_->Search(key);
}

std::pair<bool, long long> BTree::SearchSmaller(const long long key) const
{
	return root_ == nullptr ? std::make_pair(false, 0LL) : root_->SearchSmaller(key);
}

std::pair<bool, long long> BTree::SearchGreater(const long long key) const
{
	return root_ == nullptr ? std::make_pair(false, 0LL) : root_->SearchGreater(key);
}

void BTree::PrintBetween(const long long key1, const long long key2, std::ostream &os)
{
	root_->PrintBetween(key1, key2, os);
}