#ifndef UTILS_H
#define UTILS_H

#include <trie.h>

Trie_Node *msort(Trie_Node *head);
Trie_Node *_split(Trie_Node *children);
Trie_Node *_merge(Trie_Node *left, Trie_Node *right);

#endif