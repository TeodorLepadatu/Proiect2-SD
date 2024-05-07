#include <iostream>

#include "avl.hpp"
#include "btree.hpp"
#include "treap.hpp"

#include <fstream>
#include <iostream>

int main()
{
	AVL avl;
	BTree btree(4);
	Treap treap;

	std::ifstream in("input.txt");
	std::ofstream out_avl("output_avl.txt");
	std::ofstream out_btree("output_btree.txt");
	std::ofstream out_treap("output_treap.txt");

	long long n;
	in >> n;
	long long min;
	long long max;
	
	for (long long i = 0; i < n; ++i) {
		long long key;
		in >> key;
		avl.Insert(key);
		btree.Insert(key);
		treap.Insert(key);

		if (i == 0) {
			min = key;
			max = key;
		} else {
			if (key < min) {
				min = key;
			}
			if (key > max) {
				max = key;
			}
		}
	}

	avl.PrintBetween(min, max, out_avl);
	btree.PrintBetween(min, max, out_btree);
	treap.PrintBetween(min, max, out_treap);

	in.close();
	out_avl.close();
	out_btree.close();
	out_treap.close();

	return 0;
}