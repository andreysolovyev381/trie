//
// Created by Andrey Solovyev on 20.03.2024.
//

#include <gtest/gtest.h>

#include "../include/trie.hpp"
#include <cstdint>

TEST (indexes, integers) {
	ASSERT_EQ(::containers::trie::details::GetIndex{}(0), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(1), 1u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::size_t>(0)), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::size_t>(1)), 1u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::int64_t>(0)), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::int64_t>(1)), 1u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::uint64_t>(0)), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::uint64_t>(1)), 1u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::int8_t>(0)), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::int8_t>(1)), 1u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::byte>(0)), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(static_cast<std::byte>(1)), 1u);
}


TEST (indexes, chars) {
	ASSERT_EQ(::containers::trie::details::GetIndex{}('a'), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}('A'), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}('B'), 1u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}('l'), 11u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}('Z'), 25u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}('x'), 23u);
}

TEST (indexes, boolean) {
	ASSERT_EQ(::containers::trie::details::GetIndex{}(false), 0u);
	ASSERT_EQ(::containers::trie::details::GetIndex{}(true), 1u);
}
