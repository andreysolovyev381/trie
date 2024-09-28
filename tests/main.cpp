//
// Created by Andrey Solovyev on 20.03.2024.
//


/**
 * integers fully tested with LeetCode 3043 Find the Length of the Longest Common Prefix
 * https://leetcode.com/problems/find-the-length-of-the-longest-common-prefix/description/
 * 718 unit tests in total
 *
 * strings fully tested with LeetCode 208 Implement Trie
 * https://leetcode.com/problems/implement-trie-prefix-tree/description/
 * 16 unit tests in total
 *
 * https://leetcode.com/problems/sum-of-prefix-scores-of-strings/description/?envType=daily-question&envId=2024-09-25
 * */

#include <gtest/gtest.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
	testing::InitGoogleTest(&argc, argv);
	testing::GTEST_FLAG(color) = "yes";
	auto res {RUN_ALL_TESTS()};
	return res;
}

//todo
