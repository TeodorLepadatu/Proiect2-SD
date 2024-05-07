#include <iostream>

#include "avl.hpp"
#include "extra/AVL.cpp"

int main()
{
	AVL avl;
	avl.Insert(11);
	avl.Insert(17);
	avl.Insert(4);
	avl.Insert(5);
	avl.Insert(18);
	avl.Insert(8);
	avl.Insert(12);

	Node* root = NULL;
	root = insert(11, root);
	root = insert(17, root);
	root = insert(4, root);
	root = insert(5, root);
	root = insert(18, root);
	root = insert(8, root);
	root = insert(12, root);

	return 0;
}