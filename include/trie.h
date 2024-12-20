#include <stdbool.h>
#include <ctype.h>

#ifndef TRIE_H
#define TRIE_H

typedef struct Trie_Node
{
    bool EOW;
    char ch;
    unsigned int freq;
    struct Trie_Node *children;
    struct Trie_Node *sibling;
} Trie_Node;

typedef struct
{
    Trie_Node *root;
} Trie;

Trie trie_constructor(void);
void trie_insert(const char *str, Trie *trie);
bool trie_search(const char *str, Trie *trie);
int trie_prefix_match(const char *prefix, Trie *trie, char **matches, int num_matches_threshold);
void trie_unload(Trie *trie);
void trie_sort_nodes(Trie *trie);

// Should be transparent
Trie_Node *_trie_create_node(char ch);
Trie_Node *_trie_find_child(Trie_Node *parent, char ch);
void _trie_transverse(Trie_Node *curr, char *buff, size_t depth, char **matches, int *num_matches_ptr, int num_matches_threshold);
void _trie_unloader(Trie_Node *curr);
void _trie_sorter(Trie_Node *curr);

#endif
