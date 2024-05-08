#include <iostream>

#include "avl.hpp"
#include "btree.hpp"
#include "treap.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>

long long FindMinKey(const std::vector<long long> &arr)
{
	long long min = std::numeric_limits<long long>::max();

	for (const auto &el : arr) {
		if (el < min) {
			min = el;
		}
	}

	return min;
}

long long FindMaxKey(const std::vector<long long> &arr)
{
	long long max = std::numeric_limits<long long>::min();

	for (const auto &el : arr) {
		if (el > max) {
			max = el;
		}
	}

	return max;
}

void GenerateArray(std::vector<long long> &arr, const int n)
{
	long long min = - (n * 2);

	for (int i = 0; i < n * 4; ++i) {
		arr.push_back(min++);
	}

	std::shuffle(arr.begin(), arr.end(), std::mt19937(std::random_device()()));

	arr.resize(n);
}

long long GreatestLessThan(const std::vector<long long> &arr, const long long key)
{
	long long max = std::numeric_limits<long long>::min();

	for (const auto &el : arr) {
		if (el < key && el > max) {
			max = el;
		}
	}

	return max;
}

long long SmallestGreaterThan(const std::vector<long long> &arr, const long long key)
{
	long long min = std::numeric_limits<long long>::max();

	for (const auto &el : arr) {
		if (el > key && el < min) {
			min = el;
		}
	}

	if (min == std::numeric_limits<long long>::max()) {
		min = std::numeric_limits<long long>::min();
	}

	return min;
}

int main(int argc, char **argv)
{
	AVL avl;
	BTree btree(4);
	Treap treap;
	
	std::vector<long long> keys;
	std::vector<long long> lookups;

	std::string dir;
	long long n;

	if (argc == 1) {
		dir = "test1";
		n = 1000;
	} else {
		dir = argv[1];
		n = std::stoll(argv[2]);
	}

	std::cout << "Generating keys...\n";
	GenerateArray(keys, n);
	std::cout << "Generating lookups...\n";
	GenerateArray(lookups, n);

	std::vector<long long> keys_sorted = keys;
	std::sort(keys_sorted.begin(), keys_sorted.end());

	long long min_key = FindMinKey(keys);
	long long max_key = FindMaxKey(keys);

	std::ofstream avl_input(dir + "/results/avl_input.txt");
	std::ofstream btree_input(dir + "/results/btree_input.txt");
	std::ofstream treap_input(dir + "/results/treap_input.txt");

	auto total_avl = 0;
	auto total_btree = 0;
	auto total_treap = 0;

	long long i = 1;

	for (const auto &key : keys) {
		auto start = std::chrono::high_resolution_clock::now();
		avl.Insert(key);
		auto end = std::chrono::high_resolution_clock::now();
		total_avl += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		start = std::chrono::high_resolution_clock::now();
		btree.Insert(key);
		end = std::chrono::high_resolution_clock::now();
		total_btree += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		start = std::chrono::high_resolution_clock::now();
		treap.Insert(key);
		end = std::chrono::high_resolution_clock::now();
		total_treap += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		if (i % (n / 25) == 0) {
			avl_input << total_avl << std::endl;
			btree_input << total_btree << std::endl;
			treap_input << total_treap << std::endl;
			
			total_avl = 0;
			total_btree = 0;
			total_treap = 0;
		}

		++i;
	}

	avl_input.close();
	btree_input.close();
	treap_input.close();

	std::ofstream avl_search(dir + "/results/avl_search.txt");
	std::ofstream btree_search(dir + "/results/btree_search.txt");
	std::ofstream treap_search(dir + "/results/treap_search.txt");
	std::ofstream avl_search_result(dir + "/results/avl_search_result.txt");
	std::ofstream btree_search_result(dir + "/results/btree_search_result.txt");
	std::ofstream treap_search_result(dir + "/results/treap_search_result.txt");
	std::ofstream result_correct(dir + "/results/result_correct.txt");

	total_avl = 0;
	total_btree = 0;
	total_treap = 0;

	i = 1;
	bool found;

	for (const auto &lookup : lookups) {
		auto start = std::chrono::high_resolution_clock::now();
		found = avl.Search(lookup) != nullptr;
		auto end = std::chrono::high_resolution_clock::now();
		total_avl += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		avl_search_result << found << std::endl;

		start = std::chrono::high_resolution_clock::now();
		found = btree.Search(lookup) != nullptr;
		end = std::chrono::high_resolution_clock::now();
		total_btree += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		btree_search_result << found << std::endl;

		start = std::chrono::high_resolution_clock::now();
		found = treap.Search(lookup) != nullptr;
		end = std::chrono::high_resolution_clock::now();
		total_treap += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		treap_search_result << found << std::endl;

		result_correct << ((keys.end() != std::find(keys.begin(), keys.end(), lookup)) ? 1 : 0) << std::endl;

		if (i % (n / 25) == 0) {
			avl_search << total_avl << std::endl;
			btree_search << total_btree << std::endl;
			treap_search << total_treap << std::endl;
			
			total_avl = 0;
			total_btree = 0;
			total_treap = 0;
		}

		++i;
	}

	avl_search.close();
	btree_search.close();
	treap_search.close();
	avl_search_result.close();
	btree_search_result.close();
	treap_search_result.close();
	result_correct.close();

	std::shuffle(lookups.begin(), lookups.end(), std::mt19937(std::random_device()()));

	std::ofstream avl_search_smaller(dir + "/results/avl_search_smaller.txt");
	std::ofstream btree_search_smaller(dir + "/results/btree_search_smaller.txt");
	std::ofstream treap_search_smaller(dir + "/results/treap_search_smaller.txt");
	std::ofstream avl_search_smaller_result(dir + "/results/avl_search_smaller_result.txt");
	std::ofstream btree_search_smaller_result(dir + "/results/btree_search_smaller_result.txt");
	std::ofstream treap_search_smaller_result(dir + "/results/treap_search_smaller_result.txt");
	std::ofstream result_correct_smaller(dir + "/results/result_correct_smaller.txt");

	total_avl = 0;
	total_btree = 0;
	total_treap = 0;

	i = 1;
	long long value;

	for (const auto &lookup : lookups) {
		auto start = std::chrono::high_resolution_clock::now();
		value = avl.GetKey(avl.SearchSmaller(lookup));
		auto end = std::chrono::high_resolution_clock::now();
		total_avl += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		avl_search_smaller_result << value << std::endl;

		start = std::chrono::high_resolution_clock::now();
		value = btree.SearchSmaller(lookup).second;
		end = std::chrono::high_resolution_clock::now();
		total_btree += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		btree_search_smaller_result << value << std::endl;

		start = std::chrono::high_resolution_clock::now();
		value = treap.GetKey(treap.SearchSmaller(lookup));
		end = std::chrono::high_resolution_clock::now();
		total_treap += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		treap_search_smaller_result << value << std::endl;

		result_correct_smaller << GreatestLessThan(keys, lookup) << std::endl;

		if (i % (n / 25) == 0) {
			avl_search_smaller << total_avl << std::endl;
			btree_search_smaller << total_btree << std::endl;
			treap_search_smaller << total_treap << std::endl;
			
			total_avl = 0;
			total_btree = 0;
			total_treap = 0;
		}

		++i;
	}

	avl_search_smaller.close();
	btree_search_smaller.close();
	treap_search_smaller.close();
	avl_search_smaller_result.close();
	btree_search_smaller_result.close();
	treap_search_smaller_result.close();
	result_correct_smaller.close();

	std::shuffle(lookups.begin(), lookups.end(), std::mt19937(std::random_device()()));

	std::ofstream avl_search_greater(dir + "/results/avl_search_greater.txt");
	std::ofstream btree_search_greater(dir + "/results/btree_search_greater.txt");
	std::ofstream treap_search_greater(dir + "/results/treap_search_greater.txt");
	std::ofstream avl_search_greater_result(dir + "/results/avl_search_greater_result.txt");
	std::ofstream btree_search_greater_result(dir + "/results/btree_search_greater_result.txt");
	std::ofstream treap_search_greater_result(dir + "/results/treap_search_greater_result.txt");
	std::ofstream result_correct_greater(dir + "/results/result_correct_greater.txt");

	total_avl = 0;
	total_btree = 0;
	total_treap = 0;

	i = 1;

	for (const auto &lookup : lookups) {
		auto start = std::chrono::high_resolution_clock::now();
		value = avl.GetKey(avl.SearchGreater(lookup));
		auto end = std::chrono::high_resolution_clock::now();
		total_avl += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		avl_search_greater_result << value << std::endl;

		start = std::chrono::high_resolution_clock::now();
		value = btree.SearchGreater(lookup).second;
		end = std::chrono::high_resolution_clock::now();
		total_btree += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		btree_search_greater_result << value << std::endl;

		start = std::chrono::high_resolution_clock::now();
		value = treap.GetKey(treap.SearchGreater(lookup));
		end = std::chrono::high_resolution_clock::now();
		total_treap += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		treap_search_greater_result << value << std::endl;

		result_correct_greater << SmallestGreaterThan(keys, lookup) << std::endl;

		if (i % (n / 25) == 0) {
			avl_search_greater << total_avl << std::endl;
			btree_search_greater << total_btree << std::endl;
			treap_search_greater << total_treap << std::endl;
			
			total_avl = 0;
			total_btree = 0;
			total_treap = 0;
		}

		++i;
	}

	avl_search_greater.close();
	btree_search_greater.close();
	treap_search_greater.close();
	avl_search_greater_result.close();
	btree_search_greater_result.close();
	treap_search_greater_result.close();
	result_correct_greater.close();

	std::ofstream avl_print_between(dir + "/results/avl_print_between.txt");
	std::ofstream btree_print_between(dir + "/results/btree_print_between.txt");
	std::ofstream treap_print_between(dir + "/results/treap_print_between.txt");
	std::ofstream avl_print_between_result(dir + "/results/avl_print_between_result.txt");
	std::ofstream btree_print_between_result(dir + "/results/btree_print_between_result.txt");
	std::ofstream treap_print_between_result(dir + "/results/treap_print_between_result.txt");

	auto start = std::chrono::high_resolution_clock::now();
	avl.PrintBetween(min_key, max_key, avl_print_between);
	auto end = std::chrono::high_resolution_clock::now();
	avl_print_between_result << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;

	start = std::chrono::high_resolution_clock::now();
	btree.PrintBetween(min_key, max_key, btree_print_between);
	end = std::chrono::high_resolution_clock::now();
	btree_print_between_result << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;

	start = std::chrono::high_resolution_clock::now();
	treap.PrintBetween(min_key, max_key, treap_print_between);
	end = std::chrono::high_resolution_clock::now();
	treap_print_between_result << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;

	avl_print_between.close();
	btree_print_between.close();
	treap_print_between.close();
	avl_print_between_result.close();
	btree_print_between_result.close();
	treap_print_between_result.close();

	std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device()()));

	std::ofstream avl_erase(dir + "/results/avl_erase.txt");
	std::ofstream btree_erase(dir + "/results/btree_erase.txt");
	std::ofstream treap_erase(dir + "/results/treap_erase.txt");

	total_avl = 0;
	total_btree = 0;
	total_treap = 0;

	i = 1;

	for (const auto &key : keys) {
		auto start = std::chrono::high_resolution_clock::now();
		avl.Erase(key);
		auto end = std::chrono::high_resolution_clock::now();
		total_avl += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		start = std::chrono::high_resolution_clock::now();
		btree.Erase(key);
		end = std::chrono::high_resolution_clock::now();
		total_btree += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		start = std::chrono::high_resolution_clock::now();
		treap.Erase(key);
		end = std::chrono::high_resolution_clock::now();
		total_treap += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		if (i % (n / 25) == 0) {
			avl_erase << total_avl << std::endl;
			btree_erase << total_btree << std::endl;
			treap_erase << total_treap << std::endl;
			
			total_avl = 0;
			total_btree = 0;
			total_treap = 0;
		}

		++i;
	}

	avl_erase.close();
	btree_erase.close();
	treap_erase.close();

	return 0;
}