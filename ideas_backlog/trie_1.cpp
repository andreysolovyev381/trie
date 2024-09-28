#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace freq {

  template<int abc_size = 128>
  struct trie_t final {
  private:
	  struct node_t final {
		  std::vector<int> next_chars; //outgoing transitions
		  std::vector<int> go; //transitions
		  int suffix_link; //suffix link
		  int parent_idx; //parent vertex
		  char parent_ch; //symbol, that is used by parent to jump to this vertex
		  bool is_leaf;

		  node_t();
	  };
  public:
	  trie_t(std::size_t);

	  void resize(std::size_t) &;
	  void insert(std::string_view)&;
	  bool contains(std::string_view) const&;
	  bool empty () const &;

  private:
	  std::size_t capacity, sz;
	  std::vector<node_t> t;
	  bool is_leaf;

	  int get_link (int v);
	  int go (int v, char c);
  };

  inline
  int cast(char c) { return static_cast<int> (c) - 97; } //for lower English abc
#define IDX_OF(i) cast(key[i])

  template<int abc_size>
  trie_t<abc_size>::node_t::node_t()
		  : next_chars(abc_size, -1)
		  , go(abc_size, -1)
		  , suffix_link(-1)
		  , parent_idx (-1)
		  , is_leaf(false)
  {}

  template<int abc_size>
  trie_t<abc_size>::trie_t(std::size_t cap)
		  : capacity(cap)
		  , sz (1)
		  , t(capacity + 1)
  {}

  //todo: add capacity vs size check
  template<int abc_size>
  void trie_t<abc_size>::insert(std::string_view key) & {
	  int const key_size{static_cast<int>(key.size())};
	  int idx {0}, v{0};
	  for (int i = 0; i < key_size; ++i) {
		  idx = IDX_OF(i);
		  if (t[v].next_chars[idx] == -1) {
			  t[sz].parent_idx = v;
			  t[sz].parent_ch = key[i];
			  t[v].next_chars[idx] = sz++;
		  }
		  v = t[v].next_chars[idx];
	  }
	  t[v].is_leaf = true;
  }


  template<int abc_size>
  bool trie_t<abc_size>::contains(std::string_view key) const & {
	  int const key_size{static_cast<int>(key.size())};
	  int next {0}, v{0};
	  for (int i = 0; i < key_size; ++i) {
		  next = t[v].next_chars[IDX_OF(i)];
		  if (next == -1) return false;
		  else v = next;
	  }
	  // return node ptr if the current node is a leaf and the
	  // end of the string is reached
	  return t[v].is_leaf ? true : false;
  }


  template<int abc_size>
  bool trie_t<abc_size>::empty () const & {
	  return sz == 1;
  }

  template<int abc_size>
  int trie_t<abc_size>::get_link (int v) {
	  if (t[v].suffix_link == -1) {
		  if (v==0 || t[v].parent_idx==0) {
			  t[v].suffix_link = 0;
		  }
		  else {
			  t[v].suffix_link = go(get_link(t[v].parent_idx), t[v].parent_ch);
		  }
	  }
	  return t[v].suffix_link;
  }

  template<int abc_size>
  int trie_t<abc_size>::go (int v, char c) {
	  int idx = cast(c);
	  if (t[v].go[idx] == -1) {
		  if (t[v].next_chars[idx]!=-1) {
			  t[v].go[idx] = t[v].next_chars[idx];
		  }
		  else {
			  t[v].go[idx] = v==0 ? 0 : go(get_link(v), c);
		  }
	  }
	  return t[v].go[idx];
  }
#undef IDX_OF

}//!namespace

int main() {
	using namespace freq;

	trie_t<26> trie (20);

	trie.insert("hello");
	std::cout << trie.contains("hello") << " ";      // print 1

	trie.insert("helloworld");
	std::cout << trie.contains("helloworld") << " "; // print 1

	std::cout << trie.contains("helll") << " ";      // print 0 (Not found)

	trie.insert("hell");
	std::cout << trie.contains("hell") << " ";       // print 1

	trie.insert("h");
	std::cout << trie.contains("h") << " ";          // print 1

	trie.insert("fuck");
	std::cout << trie.contains("fuck") << " ";       // print 1
	std::cout << trie.contains("uck") << " ";        // print 0
	std::cout << trie.contains("fuc") << " ";        // print 0
	std::cout << trie.contains("c") << " ";          // print 0

	std::cout << std::endl;

	return 0;
}