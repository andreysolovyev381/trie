## Trie implementation

### Rational
I am redoing LeetCode [design list](leetcode.com/problem-list/aasn24ue/).
I am normally lazy and I don't like to do the things over and over. A Trie is required by a number of tasks at LeetCode, so this one fits all.
Here is the list of the tasks I used this implementation to submit solutions for:
* [LeetCode 208](https://leetcode.com/problems/implement-trie-prefix-tree/description/) Implement Trie
* [LeetCode 3043](https://leetcode.com/problems/find-the-length-of-the-longest-common-prefix/description/) Find the Length of the Longest Common Prefix

 
 * not this yet, but maybe I will adopt some payload type mixin 
 * https://leetcode.com/problems/sum-of-prefix-scores-of-strings/description/?envType=daily-question&envId=2024-09-25
 * 

### Usage examples
```cpp
#include "include/trie.hpp"
...

    ::containers::trie::of_int trie;
    tries.insert(std::vector<int>{4, 2});
	
	trie.find_word(std::vector<int>{4, 3});
	trie.find_prefix(std::vector<int>{4}); //returns longest found prefix in a form of vector
	
	std::vector<int>{4, 2} prefix;
	trie.is_prefix(prefix.begin(), prefix.end(); //returns if a Container is a prefix 

	//...
    ::containers::trie::of_char trie_str;
    ::containers::trie::of_bool trie_bits;
    
	// general declaration
	// please pay attention GetIndexFunc should be noexcept 
	//
	//template<typename T, typename GetIndexFunc, std::size_t ABCSize>
	//using general = details::Trie<T, GetIndexFunc, ABCSize>;
    auto get_index = [](char c) noexcept {return static_cast<std::size_t>(c - 'A');};
	::containers::trie::general<char, decltype(get_index), 26u> trie_upper_chars;
```


### License
MIT

### Disclosure
Despite heavy testing performed (see ./tests/data), no guarantees of any kind are given whatsoever. Use it at your own risk.
