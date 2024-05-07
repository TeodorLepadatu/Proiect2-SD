#ifndef BTREENODE_HPP
#define BTREENODE_HPP

#include <algorithm>
#include <cstdlib>
#include <ostream>

class BTreeNode {
public:
	BTreeNode(const size_t min_degree, const bool leaf);
	void Traverse(std::ostream &os) const;



	friend class BTree;

private:
	void SplitChild(const size_t idx, BTreeNode *child);
	void InsertNonFull(const long long key);
	void Erase(const long long key);
	void EraseFromLeaf(const size_t idx);
	void EraseFromNonLeaf(const size_t idx);
	size_t FindKey(const long long key) const;
	void Fill(const size_t idx);
	void Merge(const size_t idx);
	void BorrowFromPrev(const size_t idx);
	void BorrowFromNext(const size_t idx);
	BTreeNode* Search(const long long key);
	std::pair<bool, long long> SearchSmaller(const long long key);
	std::pair<bool, long long> SearchGreater(const long long key);
	long long RightMostKey(const long long key) const;
	long long LeftMostKey(const long long key) const;
	void PrintBetween(const long long key1, const long long key2, std::ostream &os);

	long long GetPredecessor(const size_t idx) const;
	long long GetSuccessor(const size_t idx) const;


	long long *keys_;
	const size_t min_degree_;
	BTreeNode **children_;
	size_t key_count_;
	bool leaf_;
};

#endif //BTREENODE_HPP