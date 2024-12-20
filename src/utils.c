#include <stdlib.h>
#include <trie.h>
#include <utils.h>

Trie_Node *msort(Trie_Node *children)
{
    if (children == NULL || children->sibling == NULL)
        return children;

    Trie_Node *half = _split(children);

    children = msort(children);
    half = msort(half);
    return _merge(children, half);
}

Trie_Node *_split(Trie_Node *children)
{
    if (children == NULL || children->sibling == NULL)
        return NULL;

    Trie_Node *slow = children;
    Trie_Node *fast = children;

    while (fast->sibling != NULL && fast->sibling->sibling != NULL)
    {
        slow = slow->sibling;
        fast = fast->sibling->sibling;
    }
    Trie_Node *half = slow->sibling ;
    slow->sibling = NULL;

    return half;
}

Trie_Node *_merge(Trie_Node *left, Trie_Node *right)
{
    if (left == NULL)
        return right;
    if (right == NULL)
        return left;

    Trie_Node *dummy;
    if (left->freq > right->freq)
    {
        dummy = left;
        left = left->sibling;
    }
    else
    {
        dummy = right;
        right = right->sibling;
    }
 
    Trie_Node *curr;
    for (curr = dummy; left != NULL && right != NULL; curr = curr->sibling)
    {
        if (left->freq > right->freq)
        {
            curr->sibling = left;
            left = left->sibling;
        }
        else
        {
            curr->sibling = right;
            right = right->sibling;
        }
    }
    curr->sibling = left != NULL ? left : right;
    return dummy;
}