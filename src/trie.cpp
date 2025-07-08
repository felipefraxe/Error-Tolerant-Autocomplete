#include <queue>

#include "trie.hpp"


using std::queue;
using std::make_shared;


void Trie::insert(string_view str)
{
    TrieNode *curr = this->root.get();
    for (char ch : str)
    {
        ch = tolower(ch);
        if (curr->children.contains(ch) == false)
            curr->children[ch] = make_unique<TrieNode>();
        curr = curr->children[ch].get();
    }
    curr->is_word = true;
}

bool Trie::search(string_view str)
{
    TrieNode *curr = this->root.get();
    for (char ch : str)
    {
        ch = tolower(ch);
        if (curr->children.contains(ch) == false)
            return false;
        curr = curr->children[ch].get();
    }
    return curr->is_word;
}

vector<string> Trie::traverse(string_view str, EditVectorAutomata& eva, Bitmap& bitmap)
{
    vector<string> matches;

    queue<ActiveNode> active_nodes;
    active_nodes.push(ActiveNode(this->root->children, "", 1, 0));
    while (!active_nodes.empty())
    {
        ActiveNode active_node = active_nodes.front();
        active_nodes.pop();
        for (auto& [ch, child] : active_node.children)
        {
            unsigned int bitmask = bitmap.extract_bitmask(ch, active_node.level, eva.editvec_length);
            unsigned int state = eva.get_next_state(active_node.state, bitmask);
            if (state == 0)
                continue;

            string prefix = active_node.prefix + ch;
            if (prefix.length() < str.length() - eva.edit_distance_threshold)
                active_nodes.push(ActiveNode(child->children, prefix, state, active_node.level + 1));
            else if (prefix.length() <= str.length() + eva.edit_distance_threshold)
            {
                vector<unsigned int> editvec = eva.get_editvec(state);
                unsigned int idx = eva.edit_distance_threshold + str.length() - prefix.length();
                unsigned int edit_distance = editvec[idx];
                if (edit_distance <= eva.edit_distance_threshold)
                {
                    if (prefix.length() >= str.length())
                        this->collect_words(child.get(), prefix, matches);
                    else if (child->is_word)
                        matches.push_back(prefix);
                    else
                        active_nodes.push(ActiveNode(child->children, prefix, state, active_node.level + 1));
                }
                else
                    active_nodes.push(ActiveNode(child->children, prefix, state, active_node.level + 1));
            }
        }
    }

    return matches;
}

void Trie::collect_words(TrieNode *node, string& prefix, vector<string>& words)
{
    if (node->is_word)
        words.push_back(prefix);
    
    for (auto& [ch, child] : node->children)
    {
        prefix += ch;
        this->collect_words(child.get(), prefix, words);
        prefix.pop_back();
    }
}
