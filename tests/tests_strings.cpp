//
// Created by Andrey Solovyev on 20.03.2024.
//

#include <gtest/gtest.h>

#include "../include/trie.hpp"

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

auto get_chars = [](
		std::vector<std::string> const& actions,
		std::vector<std::string> const& inputs)
{
	::containers::trie::of_char trie;
	std::vector<std::string> actual_res;

	for (int idx = 0; auto const& action : actions) {
		if (action == "insert") {
			trie.insert(inputs[idx]);
			actual_res.push_back("null");
		}
		else if (action == "search") {
			actual_res.push_back(trie.find_word(inputs[idx]) ? "true" : "false");
		}
		else if (action == "startsWith") {
			actual_res.push_back(trie.is_prefix(inputs[idx]) ? "true" : "false");
		}
		++idx;
	}
	return actual_res;
};


TEST(strings, t1) {
	std::vector<std::string>
			actions {"insert","search","search","startsWith","insert","search"},
			inputs {{"apple"},{"apple"},{"app"},{"app"},{"app"},{"app"}},
			expected_res {"null", "true","false","true","null", "true"},
			actual_res {get_chars(actions, inputs)};

	for (std::size_t i = 0; i != expected_res.size(); ++i) {
		if (expected_res[i] != actual_res[i]) {
			std::cout << std::boolalpha << ' ' << i << ' ' << expected_res[i] << ' ' << actual_res[i] << '\n';
		}
	}
	ASSERT_EQ(expected_res, actual_res);
}

TEST(strings, t2) {
	std::vector<std::string>
			actions {"insert","search","search","search","startsWith","startsWith","startsWith"},
			inputs {"hello",  "hell",  "helloa","hello", "hell",      "helloa",    "hello"},
			expected_res {"null", "false","false","true","true","false","true"},
			actual_res {get_chars(actions, inputs)};

	for (std::size_t i = 0; i != expected_res.size(); ++i) {
		if (expected_res[i] != actual_res[i]) {
			std::cout << std::boolalpha << ' ' << i << ' ' << expected_res[i] << ' ' << actual_res[i] << '\n';
		}
	}
	ASSERT_EQ(expected_res, actual_res);
}

auto get_input_from = [](std::filesystem::path const &input_file){
	std::vector<std::string> res;
	std::fstream f (input_file, std::ios::in);
	if (!f) {
		std::fprintf(stderr, "Haven't been able to open an input file %s\n", input_file.c_str());
		return res;
	}
	std::string input;
	while (f >> input) res.push_back(input);
	return res;
};

TEST(strings, t3_big_test) {
	auto const tests_data_folder {std::filesystem::path (CMAKE_SOURCE_DIR) / "tests/data"};

	auto const input_actions_file {tests_data_folder / "input_actions.txt"};
	auto const actions {get_input_from(input_actions_file)};

	auto const input_values_file {tests_data_folder / "input_values.txt"};
	auto const inputs {get_input_from(input_values_file)};

	auto const output_values_file {tests_data_folder / "output_expected.txt"};
	auto const expected_res {get_input_from(output_values_file)};

	auto const actual_res {get_chars(actions, inputs)};

	int error_count{0};
	for (std::size_t i = 0; i != expected_res.size(); ++i) {
		if (expected_res[i] != actual_res[i]) {
			std::cout << std::boolalpha << ' ' << i << ' ' << expected_res[i] << ' ' << actual_res[i] << '\n';
			if (++error_count > 5) break;
		}
	}
	ASSERT_EQ(expected_res, actual_res);
}


auto search = [](std::string const& searchWord, auto const& trie) {
	auto const longest_prefix {trie.find_prefix(searchWord)};
	std::size_t const
			prefix_sz {longest_prefix.size()},
			word_sz {searchWord.size()};
	if (longest_prefix.size() == searchWord.size()) {
		return false;
	}

	std::size_t const res_symbols_count {word_sz - prefix_sz};
	auto const reversed_prefix
			{trie.find_prefix(searchWord.crbegin(),
			                  searchWord.crbegin() + res_symbols_count)};
	return (not reversed_prefix.empty()) && res_symbols_count == (reversed_prefix.size() + 1);
};

TEST(strings, t4) {

	std::vector<std::string>
			dictionary {"hello","leetcode"},
			input {"hello", "hhllo", "hell", "leetcoded"};
	std::vector<bool>
			expected_res {false, true, false, false};

	::containers::trie::of_char trie;
	for (auto const& word : dictionary) {
		trie.insert(word.cbegin(), word.cend());
		trie.insert(word.crbegin(), word.crend());
	}

	for (std::size_t i = 0; i != input.size(); ++i) {
		bool const actual_res {search(input[i], trie)};
		ASSERT_EQ(expected_res[i], actual_res) << std::boolalpha << ' ' << i << ' ' << input[i] << ' ' << expected_res[i] << ' ' << actual_res << '\n';
	}
}

TEST(strings, t5) {
	auto get_index = [](char c) noexcept {return static_cast<std::size_t>(c - 'A');};
	::containers::trie::general<char, decltype(get_index), 26u> trie_upper_chars;

	trie_upper_chars.insert(std::string("ABBA"));
	ASSERT_TRUE(trie_upper_chars.find_word(std::string("ABBA")));
	ASSERT_FALSE(trie_upper_chars.find_word(std::string("AB")));
	ASSERT_TRUE(trie_upper_chars.is_prefix(std::string("AB")));
	ASSERT_FALSE(trie_upper_chars.is_prefix(std::string("ABBACC")));
}
