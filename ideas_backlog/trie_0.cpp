#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace freq {
  template<int abc_size = 128>
  class trie_t final {
  private:
	  using node_t = trie_t<abc_size>;
	  using ptr_unique_t = typename std::unique_ptr<node_t>;
	  using ptr_t = node_t*;

  public:
	  trie_t();
	  trie_t(trie_t const&) = delete;
	  trie_t operator=(trie_t const&) = delete;

	  void insert(std::string_view)&;
	  ptr_t find(std::string_view)&;
	  bool contains(std::string_view) const&;
	  bool remove(std::string_view)&;
	  bool empty () const &;

  private:
	  std::vector<ptr_unique_t> next_chars;
	  bool is_leaf;

	  bool have_children(ptr_t const) const &;
	  bool remove_impl(ptr_t, std::string_view)&;
	  ptr_t find(std::string_view) const&;

  };

  inline
  int cast(char c) { return static_cast<int> (c) - 97; } //for lower English abc
#define IDX_OF(i) cast(key[i])

  template<int abc_size>
  trie_t<abc_size>::trie_t() :is_leaf(false) {
	  next_chars.resize(abc_size);
  }

  template<int abc_size>
  void trie_t<abc_size>::insert(std::string_view key) & {
	  auto curr = this;
	  int const sz{static_cast<int>(key.size())};
	  int idx {0};
	  for (int i = 0; i < sz; ++i) {
		  idx = IDX_OF(i);
		  if (!curr->next_chars[idx]) {
			  curr->next_chars[idx] = std::make_unique<node_t>();
		  }
		  curr = curr->next_chars[idx].get();
	  }
	  curr->is_leaf = true;
  }

  template<int abc_size>
  typename trie_t<abc_size>::ptr_t
  trie_t<abc_size>::find(std::string_view key) & {
	  auto curr = this;
	  int const sz{static_cast<int>(key.size())};
	  for (int i = 0; i<sz; ++i) {
		  curr = curr->next_chars[IDX_OF(i)].get();
		  if (!curr) return nullptr;
	  }
	  // return node ptr if the current node is a leaf and the
	  // end of the string is reached
	  return curr->is_leaf ? curr : nullptr;
  }

  template<int abc_size>
  typename trie_t<abc_size>::ptr_t
  trie_t<abc_size>::find(std::string_view key) const & {
	  auto curr = this;
	  int const sz{static_cast<int>(key.size())};
	  for (int i = 0; i<sz; ++i) {
		  curr = curr->next_chars[IDX_OF(i)].get();
		  if (!curr) return nullptr;
	  }
	  // return node ptr if the current node is a leaf and the
	  // end of the string is reached
	  return curr->is_leaf ? (ptr_t const)curr : nullptr;
  }

  template<int abc_size>
  bool trie_t<abc_size>::contains(std::string_view key) const & {
	  return find(key);
  }

  template<int abc_size>
  bool trie_t<abc_size>::remove(std::string_view key) & {

//	  auto curr = this;
//	  int const sz{static_cast<int>(key.size())};
//	  for (int i = 0; i<sz; ++i) {
//		  curr = curr->character[IDX(i)].get();
//		  if (!curr) return nullptr;
//	  }
//	  // return node ptr if the current node is a leaf and the
//	  // end of the string is reached
//	  return curr->is_leaf ? curr : nullptr;
//
//	  ptr_unique_t &first = character[IDX(0)];

	  if (auto p_raw = find (key); p_raw) {
		  return remove_impl (p_raw, key.substr(1));
	  }
	  else {
		  return false;
	  }
  }

  template<int abc_size>
  bool trie_t<abc_size>::remove_impl(ptr_t curr, std::string_view key) & {

	  if (!curr) return false;

	  if (!key.empty()) {
		  if (
				  curr->next_chars[IDX_OF(0)] &&
						  remove_impl(curr->next_chars[key[0]].get(), key.substr(1)) &&
						  !curr->is_leaf) {

			  if (!have_children(curr)) return true;
			  else return false;
		  }
	  }

	  // if the end of the key is reached
	  if (key.empty() && curr->is_leaf) {
		  // if the current node is a leaf node and doesn't have any children
		  if (!have_children(curr)) {
			  // delete the non-leaf parent nodes
			  return true;
		  }

			  // if the current node is a leaf node and has children
		  else {
			  // mark the current node as a non-leaf node (DON'T DELETE IT)
			  curr->is_leaf = false;
			  // don't delete its parent nodes
			  return false;
		  }
	  }
	  return false;
  }

  template<int abc_size>
  bool trie_t<abc_size>::empty () const & {
	  return !have_children((ptr_t) this);
  }

  template<int abc_size>
  bool trie_t<abc_size>::have_children(ptr_t const curr) const & {
	  for (int i = 0; i < abc_size; i++) {
		  if (curr->next_chars[i]) return true;  // child found
	  }
	  return false;
  }

#undef IDX_OF

}//!namespace

int main() {
	using namespace freq;

	trie_t<26> trie;

	trie.insert("hello");
	std::cout << trie.contains("hello") << " ";      // print 1

	trie.insert("helloworld");
	std::cout << trie.contains("helloworld") << " "; // print 1

	std::cout << trie.contains("helll") << " ";      // print 0 (Not found)

	trie.insert("hell");
	std::cout << trie.contains("hell") << " ";       // print 1

	trie.insert("h");
	std::cout << trie.contains("h");                 // print 1

	std::cout << std::endl;

	trie.remove("hello");
	std::cout << trie.contains("hello") << " ";      // print 0

	std::cout << trie.contains("helloworld") << " "; // print 1
	std::cout << trie.contains("hell");              // print 1

	std::cout << std::endl;

	trie.remove("h");
	std::cout << trie.contains("h") << " ";          // print 0
	std::cout << trie.contains("hell") << " ";       // print 1
	std::cout << trie.contains("helloworld");        // print 1

	std::cout << std::endl;

	trie.remove("helloworld");
	std::cout << trie.contains("helloworld") << " "; // print 0
	std::cout << trie.contains("hell") << " ";       // print 1

	trie.remove("hell");
	std::cout << trie.contains("hell");              // print 0

	std::cout << std::endl;

	if (trie.empty()) {
		std::cout << "Trie empty!!\n";              // Trie is empty now
	}

	std::cout << trie.contains("hell");              // print 0

	return 0;
}