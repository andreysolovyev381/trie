//
// Created by Andrey Solovyev on 20.03.2024.
//

#include <gtest/gtest.h>

#include "../include/trie.hpp"
#include <array>
#include <cstring>
#include <algorithm>

void get_digits(int n, std::array<int, 10> &digits, int& digits_count) noexcept
{
	std::memset(digits.data(), '\0', 10 * sizeof(int));
	digits_count = 0;

	if (n == 0) {
		++digits_count;
		return;
	}
	while (n > 0) {
		digits[digits_count++] = n % 10;
		n /= 10;
	}
	std::reverse(digits.begin(), digits.begin()+digits_count);
}

auto get_res = [](std::vector<int> const& arr1, std::vector<int> const& arr2){
	bool const has_more_elems {arr1.size() >= arr2.size()};
	std::vector<int> const& arr1_ {has_more_elems ? arr1 : arr2};
	std::vector<int> const& arr2_ { has_more_elems ? arr2 : arr1};

	std::array<int, 10> digits;
	int digits_count;
	::containers::trie::of_int trie;
	for (int i : arr1_) {
		get_digits(i, digits, digits_count);
		trie.insert(digits.begin(), digits.begin() + digits_count);
	}
	std::size_t res{0};
	for (int i : arr2_) {
		get_digits(i, digits, digits_count);
		res = std::max(res, trie.find_prefix(digits.begin(), digits.begin() + digits_count).size());
	}

	return res;
};

TEST(integers, t1){
	std::vector<int> arr1{1, 2, 3};
	std::vector<int> arr2{4, 4, 4};
	ASSERT_EQ(get_res(arr1, arr2), 0);
}


TEST(integers, t2){
	std::vector<int> arr1{10};
	std::vector<int> arr2{17, 11};
	ASSERT_EQ(get_res(arr1, arr2), 1);
}
