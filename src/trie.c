#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <trie.h>
#include <utils.h>

#define BUFF_LEN 2048

Trie trie_constructor(void)
{
    return (Trie)  {.root = _trie_create_node('\0')};
}

Trie_Node *_trie_create_node(char ch)
{
    Trie_Node *node = malloc(sizeof(Trie_Node));
    node->ch = ch;
    node->freq = 0;
    node->children = NULL;
    node->sibling = NULL;
    node->EOW = false;
    return node;
}

Trie_Node *_trie_find_child(Trie_Node *parent, char ch)
{
    for (Trie_Node *child = parent->children; child != NULL; child = child->sibling)
    {
        if (child->ch == ch)
            return child;
    }
    return NULL;
}

void trie_insert(const char *str, Trie *trie)
{
    Trie_Node *curr = trie->root;
    for (size_t i = 0, n = strlen(str); i < n; i++)
    {
        char ch = tolower(str[i]);
        Trie_Node *child = _trie_find_child(curr, ch);
        if (child == NULL)
        {
            child = _trie_create_node(ch);
            child->sibling = curr->children;
            curr->children = child;
        }
        curr->freq++;
        curr = child;
    }
    curr->EOW = true;
}

bool trie_search(const char *str, Trie *trie)
{
    Trie_Node *curr = trie->root;
    for (size_t i = 0, n = strlen(str); i < n; i++)
    {
        char ch = tolower(str[i]);
        Trie_Node *child = _trie_find_child(curr, ch);
        if (child == NULL)
            return false;
        curr = child;
    }
    return curr->EOW;
}

int trie_prefix_match(const char *prefix, Trie *trie, char **matches, int num_matches_threshold)
{
    int num_matches = 0;
    size_t prefix_len = strlen(prefix);
    Trie_Node *curr = trie->root;
    for (size_t i = 0; i < prefix_len; i++)
    {
        char ch = tolower(prefix[i]);
        Trie_Node *child = _trie_find_child(curr, ch);
        if (child == NULL)
            return num_matches;
        curr = child;
    }

    char buff[BUFF_LEN];
    strcpy(buff, prefix);
    _trie_transverse(curr, buff, prefix_len, matches, &num_matches, num_matches_threshold);
    
    return num_matches;
}

void _trie_transverse(Trie_Node *curr, char *buff, size_t depth, char **matches, int *num_matches_ptr, int num_matches_threshold)
{
    if (*num_matches_ptr >= num_matches_threshold)
        return;

    if (curr->EOW)
    {
        buff[depth] = '\0';
        matches[(*num_matches_ptr)++] = strdup(buff);
    }


    for (Trie_Node *child = curr->children; child != NULL; child = child->sibling)
    {
        if (child != NULL)
        {
            buff[depth] = child->ch;
            _trie_transverse(child, buff, depth + 1, matches, num_matches_ptr, num_matches_threshold);
        }
    }
}

void trie_sort_nodes(Trie *trie)
{
    if (trie != NULL)
        _trie_sorter(trie->root);
}

void _trie_sorter(Trie_Node *curr)
{
    curr->children = msort(curr->children);
    for (Trie_Node *child = curr->children; child != NULL; child = child->sibling)
        _trie_sorter(child);
}

void trie_unload(Trie *trie)
{
    _trie_unloader(trie->root);
    trie->root = NULL;
}

void _trie_unloader(Trie_Node *curr)
{
    for (Trie_Node *child = curr->children; child != NULL; child = child->sibling)
        _trie_unloader(child);
    free(curr);
}
