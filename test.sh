#!/bin/bash

g++ -O3 -o main main.cpp -I include/ src/*.cpp

for test in $(seq 1 $i); do
	for i in "1000" "100000" "1000000"; do
		mkdir -p "test$test-$i"
		mkdir -p "test$test-$i/results"
		./main "test$test-$i" $i

		touch "test$test-$i/corectness.out"

		echo -en "Search normal:\n" >> "test$test-$i/corectness.out"
		
		echo -en "AVL: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/avl_search_result.txt" "test$test-$i/results/result_correct.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "BTree: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/btree_search_result.txt" "test$test-$i/results/result_correct.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "Treap: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/treap_search_result.txt" "test$test-$i/results/result_correct.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "Search smaller:\n" >> "test$test-$i/corectness.out"

		echo -en "AVL: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/avl_search_smaller_result.txt" "test$test-$i/results/result_correct_smaller.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "BTree: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/btree_search_smaller_result.txt" "test$test-$i/results/result_correct_smaller.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "Treap: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/treap_search_smaller_result.txt" "test$test-$i/results/result_correct_smaller.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "Search greater:\n" >> "test$test-$i/corectness.out"

		echo -en "AVL: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/avl_search_greater_result.txt" "test$test-$i/results/result_correct_greater.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "BTree: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/btree_search_greater_result.txt" "test$test-$i/results/result_correct_greater.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi

		echo -en "Treap: " >> "test$test-$i/corectness.out"
		cmp "test$test-$i/results/treap_search_greater_result.txt" "test$test-$i/results/result_correct_greater.txt"
		if [ $? -eq 0 ]; then
			echo -en "OK\n" >> "test$test-$i/corectness.out"
		else
			echo -en "FAIL\n" >> "test$test-$i/corectness.out"
		fi
	done
done