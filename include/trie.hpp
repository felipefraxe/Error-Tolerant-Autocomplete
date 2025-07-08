#ifndef TRIE_HPP
#define TRIE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "bitmap.hpp"
#include "eva.hpp"


using std::unordered_map;
using std::shared_ptr;
using std::string;
using std::string_view;
using std::unique_ptr;
using std::vector;


class Trie
{
public:
    Trie() : root(make_unique<TrieNode>()) {};
    void insert(string_view str);
    bool search(string_view str);
    vector<string> traverse(string_view prefix, EditVectorAutomata& eva, Bitmap& bitmap);

private:
    struct TrieNode {
        TrieNode() : is_word(false) {}
        bool is_word;
        unordered_map<char, unique_ptr<TrieNode>> children;
    };

    struct ActiveNode {
        ActiveNode(unordered_map<char, unique_ptr<TrieNode>>& children, string_view prefix, unsigned int state, unsigned int level)
            : prefix(prefix), state(state), level(level), children(children) {}
        string prefix;
        unsigned int state;
        unsigned int level;
        unordered_map<char, unique_ptr<TrieNode>>& children;

    };

    unique_ptr<TrieNode> root;

    void collect_words(TrieNode *node, string& prefix, vector<string>& words);
};

#endif