//
// Created by Andrey Solovyev on 11/08/2023.
//

#pragma once

#include <concepts>
#include <type_traits>
#include <array>
#include <vector>
#include <memory>
#include <optional>
#include <iterator>

namespace requirements {

	namespace containers {

		template<typename C>
		concept IsContainer = requires (C c) {
			typename C::value_type;
			c.begin();
			c.end();
			c.empty();
		};

		template<typename C, typename T>
		concept IsContainerOfT = IsContainer<C> && std::same_as<typename C::value_type, T>;

	}//!namespace containers

	namespace conversion {

		template <typename Callable, typename... Args>
		concept CallableToIntegral =
				std::default_initializable<Callable> &&
				requires (Callable callable, Args... args) {
					requires std::integral<decltype(callable(args...))>;
					requires std::is_nothrow_invocable_v<Callable, Args...>;
		};

		template <typename Callable, typename... Args>
		concept CallableToIndex =
				CallableToIntegral<Callable, Args...> &&
				requires (Callable callable, Args... args) {
					requires !std::same_as<decltype(callable(args...)), bool>;
					requires !std::same_as<decltype(callable(args...)), char>;
		};

	}//!namespace conversion

}//!namespace requirements

namespace containers {

	namespace trie {

		namespace details {

			template<typename T, typename GetIndex, std::size_t abc_size = 26u>
			requires requirements::conversion::CallableToIndex<GetIndex, T>
			class Trie final {
			public:
				using value_type = T;
				static constexpr std::size_t k_abc_size {abc_size};

			private:

				struct node_t final {
					std::vector<std::unique_ptr<node_t>> next_level;
					bool is_leaf;

					node_t() : is_leaf(false) {}
				};

				struct Word {};
				struct Prefix {};

			public:

				Trie () : root (std::make_unique<node_t>())
				{}

				template <typename Container>
				requires requirements::containers::IsContainerOfT<Container, T>
				Trie (Container const& c) : root (std::make_unique<node_t>())
				{
					insert(c);
				}

				template <typename Container>
				requires requirements::containers::IsContainerOfT<Container, T>
				void insert (Container const& c) const {
					if (c.empty()) return;
					insert_(c.begin(), c.end());
				}

				template <std::input_iterator Iter>
				requires std::same_as<typename std::iter_value_t<Iter>, T>
				void insert (Iter first, Iter last) const {
					if (first >= last) return;
					insert_(first, last);
				}

				template <typename Container>
				requires requirements::containers::IsContainerOfT<Container, T>
				bool find_word(Container const& word) const noexcept {
					return !word.empty() && find_<Word>(word.begin(), word.end());
				}

				template <std::input_iterator Iter>
				requires std::same_as<typename std::iter_value_t<Iter>, T>
				bool find_word(Iter first, Iter last) const noexcept {
					return first < last && find_<Word>(first, last);
				}

				template <typename Container>
				requires requirements::containers::IsContainerOfT<Container, T>
				std::vector<T> find_prefix(Container const& prefix) const noexcept {
					return prefix.empty() ? std::vector<T>{} : find_<Prefix>(prefix.begin(), prefix.end());
				}

				template <std::input_iterator Iter>
				requires std::same_as<typename std::iter_value_t<Iter>, T>
				std::vector<T> find_prefix(Iter first, Iter last) const noexcept {
					return first < last ? find_<Prefix>(first, last) : std::vector<T>{};
				}

				template <typename Container>
				requires requirements::containers::IsContainerOfT<Container, T>
				bool is_prefix(Container const& prefix) const noexcept {
					return !prefix.empty() && find_<Prefix>(prefix.begin(), prefix.end()).size() == prefix.size();
				}

				template <std::input_iterator Iter>
				requires std::same_as<typename std::iter_value_t<Iter>, T>
				bool is_prefix(Iter first, Iter last) const noexcept {
					return first < last && find_<Prefix>(first, last).size() == std::distance(first, last);
				}

			private:
				std::unique_ptr<node_t> root;
				GetIndex get_idx;

			private:

				std::optional<std::size_t> index(T const& t) const noexcept {
					std::size_t idx {get_idx(t)};
					return idx >= abc_size ? std::nullopt : std::optional<std::size_t>{idx};
				}

				template <std::input_iterator Iter>
				void insert_ (Iter first, Iter last) const {
					node_t *node {root.get()};
					for (auto it = first; it != last; ++it) {
						auto const idx {index(*it)};
						if (!idx) break;
                        std::size_t const curr_idx {idx.value()};
						if (node->next_level.size() <= curr_idx) {
							node->next_level.resize(curr_idx + 1);
						}
						if (!node->next_level[curr_idx]) {
							node->next_level[curr_idx] = std::make_unique<node_t>();
						}
						node = node->next_level[curr_idx].get();
					}
					node->is_leaf = true;
				}


				template <typename FindMode, std::input_iterator Iter>
				auto find_(Iter first, Iter last) const noexcept {
					bool is_found {true};
					node_t *node {root.get()};

					[[maybe_unused]] std::vector<T> res;
					res.reserve(1 << 7);

					for (auto it = first; it != last; ++it) {
						auto const idx {index(*it)};
						if (!idx || node->next_level.size() <= idx.value() || !node->next_level[idx.value()]) {
							is_found = false;
							break;
						}
						if constexpr (std::is_same_v<FindMode, Prefix>) {
							res.push_back(*it);
						}
						node = node->next_level[idx.value()].get();
					}
					if constexpr (std::is_same_v<FindMode, Word>) {
						return is_found && node->is_leaf;
					}
					else if constexpr (std::is_same_v<FindMode, Prefix>) {
						return res;
					}
					else {
						static_assert(false, "Class was modified erroneously, check the changes made");
						return 42;
					}
				}
			};

			struct GetIndex {
				template <typename T>
				std::size_t operator()(T) const noexcept {
					static_assert(false, "One should use or add a specialization\n");
					return 0lu;
				}
				template <typename T>
				requires (std::is_nothrow_constructible_v<std::size_t, T> || std::is_nothrow_convertible_v<T, std::size_t>)
				std::size_t operator()(T t) const noexcept {
					return std::size_t(t);
				}
				std::size_t operator()(char c) const noexcept {
					char c_ {c < 97 ? (char)(c ^ 32) : c};
					return c_ - 'a';
				}
				std::size_t operator()(std::byte byte) const noexcept {
					return static_cast<std::size_t>(byte);
				}
			};
		}//!namespace details

		using of_char = details::Trie<char, details::GetIndex, 26u>;

		using of_int = details::Trie<int, details::GetIndex, 10u>;

		using of_bool = details::Trie<bool, details::GetIndex, 2u>;

		template<typename T, typename GetIndexFunc, std::size_t ABCSize>
		using general = details::Trie<T, GetIndexFunc, ABCSize>;

	}//!namespace trie

}//!namespace containers
