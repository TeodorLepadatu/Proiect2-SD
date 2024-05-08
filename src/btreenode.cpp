#include "btreenode.hpp"

BTreeNode::BTreeNode(const size_t min_degree, const bool leaf)
	: min_degree_(min_degree), leaf_(leaf)
{
	keys_ = new long long[2 * min_degree_ - 1];
	children_ = new BTreeNode*[2 * min_degree_];
	key_count_ = 0;
}

void BTreeNode::Traverse(std::ostream &os) const
{
	size_t i;
	for (i = 0; i < key_count_; ++i) {
		if (leaf_ == false) {
			children_[i]->Traverse(os);
		}
		os << " " << keys_[i];
	}
	if (leaf_ == false) {
		children_[i]->Traverse(os);
	}
}

void BTreeNode::SplitChild(const size_t idx, BTreeNode *child)
{
	BTreeNode *new_child = new BTreeNode(child->min_degree_, child->leaf_);
	new_child->key_count_ = min_degree_ - 1;

	for (size_t i = 0; i < min_degree_ - 1; ++i) {
		new_child->keys_[i] = child->keys_[i + min_degree_];
	}

	if (child->leaf_ == false) {
		for (size_t i = 0; i < min_degree_; ++i) {
			new_child->children_[i] = child->children_[i + min_degree_];
		}
	}

	child->key_count_ = min_degree_ - 1;
	
	for (size_t i = key_count_; i > idx; --i) {
		children_[i + 1] = children_[i];
	}

	children_[idx + 1] = new_child;

	for (size_t i = key_count_; i > idx; --i) {
		keys_[i] = keys_[i - 1];
	}

	keys_[idx] = child->keys_[min_degree_ - 1];

	++key_count_;
}

void BTreeNode::InsertNonFull(const long long key)
{
	long long i = key_count_ - 1;

	if (leaf_ == true) {
		while (i >= 0 && keys_[i] > key) {
			keys_[i + 1] = keys_[i];
			--i;
		}

		keys_[i + 1] = key;
		++key_count_;
	} else {
		while (i >= 0 && keys_[i] > key) {
			--i;
		}

		if (children_[i + 1]->key_count_ == 2 * min_degree_ - 1) {
			SplitChild(i + 1, children_[i + 1]);

			if (keys_[i + 1] < key) {
				++i;
			}
		}

		children_[i + 1]->InsertNonFull(key);
	}
}

void BTreeNode::Erase(const long long key)
{
	size_t idx = FindKey(key);

	if (idx < key_count_ && keys_[idx] == key) {
		if (leaf_ == true) {
			EraseFromLeaf(idx);
		} else {
			EraseFromNonLeaf(idx);
		}
	} else {
		// Key not found in the tree
		if (leaf_ == true) {
			return;
		}

		// flag = true <=> key > keys[key_count_ - 1]
		bool flag = (idx == key_count_);

		if (children_[idx]->key_count_ < min_degree_) {
			Fill(idx);
		}

		if (flag && idx > key_count_) {
			children_[idx - 1]->Erase(key);
		} else {
			children_[idx]->Erase(key);
		}
	}
}

void BTreeNode::EraseFromLeaf(const size_t idx)
{
	for (size_t i = idx + 1; i < key_count_; ++i) {
		keys_[i - 1] = keys_[i];
	}

	--key_count_;
}

void BTreeNode::EraseFromNonLeaf(const size_t idx)
{
	long long key = keys_[idx];

	if (children_[idx]->key_count_ >= min_degree_) {
		long long predecessor = GetPredecessor(idx);
		keys_[idx] = predecessor;
		children_[idx]->Erase(predecessor);
	} else if (children_[idx + 1]->key_count_ >= min_degree_) {
		long long successor = GetSuccessor(idx);
		keys_[idx] = successor;
		children_[idx + 1]->Erase(successor);
	} else {
		Merge(idx);
		children_[idx]->Erase(key);
	}
}

size_t BTreeNode::FindKey(const long long key) const
{
	size_t idx = 0;
	while (idx < key_count_ && keys_[idx] < key) {
		++idx;
	}
	return idx;
}

void BTreeNode::Fill(const size_t idx)
{
	if (idx != 0 && children_[idx - 1]->key_count_ >= min_degree_) {
		BorrowFromPrev(idx);
	} else if (idx != key_count_ && children_[idx + 1]->key_count_ >= min_degree_) {
		BorrowFromNext(idx);
	} else {
		if (idx != key_count_) {
			Merge(idx);
		} else {
			Merge(idx - 1);
		}
	}
}

void BTreeNode::Merge(const size_t idx)
{
	BTreeNode *child = children_[idx];
	BTreeNode *sibling = children_[idx + 1];

	child->keys_[min_degree_ - 1] = keys_[idx];

	for (size_t i = 0; i < sibling->key_count_; ++i) {
		child->keys_[i + min_degree_] = sibling->keys_[i];
	}

	if (child->leaf_ == false) {
		for (size_t i = 0; i <= sibling->key_count_; ++i) {
			child->children_[i + min_degree_] = sibling->children_[i];
		}
	}

	for (size_t i = idx + 1; i < key_count_; ++i) {
		keys_[i - 1] = keys_[i];
	}

	for (size_t i = idx + 2; i <= key_count_; ++i) {
		children_[i - 1] = children_[i];
	}

	child->key_count_ += sibling->key_count_ + 1;
	--key_count_;

	delete sibling;
}

void BTreeNode::BorrowFromPrev(const size_t idx)
{
	BTreeNode *child = children_[idx];
	BTreeNode *sibling = children_[idx - 1];

	for (long long i = child->key_count_ - 1; i >= 0; --i) {
		child->keys_[i + 1] = child->keys_[i];
	}

	if (child->leaf_ == false) {
		for (long long i = child->key_count_; i >= 0; --i) {
			child->children_[i + 1] = child->children_[i];
		}
	}

	child->keys_[0] = keys_[idx - 1];

	if (child->leaf_ == false) {
		child->children_[0] = sibling->children_[sibling->key_count_];
	}

	keys_[idx - 1] = sibling->keys_[sibling->key_count_ - 1];

	++child->key_count_;
	--sibling->key_count_;
}

void BTreeNode::BorrowFromNext(const size_t idx)
{
	BTreeNode *child = children_[idx];
	BTreeNode *sibling = children_[idx + 1];

	child->keys_[child->key_count_] = keys_[idx];

	if (child->leaf_ == false) {
		child->children_[child->key_count_ + 1] = sibling->children_[0];
	}

	keys_[idx] = sibling->keys_[0];

	for (size_t i = 1; i < sibling->key_count_; ++i) {
		sibling->keys_[i - 1] = sibling->keys_[i];
	}

	if (sibling->leaf_ == false) {
		for (size_t i = 1; i <= sibling->key_count_; ++i) {
			sibling->children_[i - 1] = sibling->children_[i];
		}
	}

	++child->key_count_;
	--sibling->key_count_;
}

BTreeNode* BTreeNode::Search(const long long key)
{
	size_t idx = 0;
	while (idx < key_count_ && keys_[idx] < key) {
		++idx;
	}

	if (idx < key_count_ && keys_[idx] == key) {
		return this;
	}

	if (leaf_ == true) {
		return nullptr;
	}

	return children_[idx]->Search(key);
}

std::pair<bool, long long> BTreeNode::SearchSmaller(const long long key)
{
	long long rightmost_key = RightMostKey(key);
	if (rightmost_key == -1) {
		if (leaf_ == true) {
			return std::make_pair(false, -1);
		} else {
			return children_[0]->SearchSmaller(key);
		}
	} else {
		if (leaf_ == true) {
			return std::make_pair(true, keys_[rightmost_key]);
		} else {
			return children_[rightmost_key + 1]->SearchSmaller(key);
		}
	}
}

std::pair<bool, long long> BTreeNode::SearchGreater(const long long key)
{
	long long leftmost_key = LeftMostKey(key);
	if (leftmost_key == -1) {
		if (leaf_ == true) {
			return std::make_pair(false, -1);
		} else {
			return children_[0]->SearchGreater(key);
		}
	} else {
		if (leaf_ == true) {
			return std::make_pair(true, keys_[leftmost_key]);
		} else {
			return children_[leftmost_key]->SearchGreater(key);
		}
	}
}

long long BTreeNode::RightMostKey(const long long key) const
{
	long long left = 0;
	long long right = key_count_ - 1;
	long long result = -1;

	while (left <= right) {
		long long mid = left + (right - left) / 2;
		if (keys_[mid] < key) {
			result = mid;
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	return result;
}

long long BTreeNode::LeftMostKey(const long long key) const
{
	long long left = 0;
	long long right = key_count_ - 1;
	long long result = -1;

	while (left <= right) {
		long long mid = left + (right - left) / 2;
		if (keys_[mid] > key) {
			result = mid;
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}

	return result;
}

void BTreeNode::PrintBetween(const long long key1, const long long key2, std::ostream &os)
{
	for (size_t i = 0; i < key_count_; ++i) {
		if (keys_[i] >= key1) {
			if (leaf_ == false) {
				children_[i]->PrintBetween(key1, key2, os);
			}
			if (keys_[i] <= key2) {
				os << keys_[i] << " ";
			}
		
		}
	}

	if (leaf_ == false) {
		children_[key_count_]->PrintBetween(key1, key2, os);
	}
}

long long BTreeNode::GetPredecessor(const size_t idx) const
{
	BTreeNode *current = children_[idx];
	while (current->leaf_ == false) {
		current = current->children_[current->key_count_];
	}
	return current->keys_[current->key_count_ - 1];
}

long long BTreeNode::GetSuccessor(const size_t idx) const
{
	BTreeNode *current = children_[idx + 1];
	while (current->leaf_ == false) {
		current = current->children_[0];
	}
	return current->keys_[0];
}