#include <stdbool.h>
#include <ctype.h>

#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 26

typedef struct Trie_Node
{
    bool EOW;
    char ch;
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

// Should be transparent
Trie_Node *trie_create_node(char ch);
Trie_Node *trie_find_child(Trie_Node *parent, char ch);
void trie_transverse(Trie_Node *curr, char *buff, size_t depth, char **matches, int *num_matches_ptr, int num_matches_threshold);
void trie_unloader(Trie_Node *curr);

#endif
