#ifndef BTREE_HPP
#define BTREE_HPP

#include "btreenode.hpp"
#include <ostream>

class BTree {
public:
	BTree(const size_t min_degree);

	//void Traverse() const;
	void Insert(const long long key);
	void Erase(const long long key);
	BTreeNode* Search(const long long key) const;
	std::pair<bool, long long> SearchSmaller(const long long key) const;
	std::pair<bool, long long> SearchGreater(const long long key) const;
	void PrintBetween(const long long key1, const long long key2, std::ostream &os);

private:
	BTreeNode *root_;
	const size_t min_degree_;
};

#endif //BTREE_HPP