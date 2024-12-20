#include <stdio.h>
#include <stdlib.h>


// Define Trie_Node structure
typedef struct Trie_Node {
    char ch;            // Character in the trie node
    int freq;           // Frequency of the character in the trie
    struct Trie_Node *sibling; // Pointer to the next sibling node
} Trie_Node;

Trie_Node *msort(Trie_Node *head);
Trie_Node *_split(Trie_Node *children);
Trie_Node *_merge(Trie_Node *left, Trie_Node *right);

// Function to create a new Trie_Node
Trie_Node* create_node(char ch, int freq) {
    Trie_Node *node = (Trie_Node*)malloc(sizeof(Trie_Node));
    node->ch = ch;
    node->freq = freq;
    node->sibling = NULL;
    return node;
}

// Merge sort function to sort Trie nodes by frequency

// Helper function to split the list into two halves
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
    if (left->freq < right->freq)
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
        if (left->freq < right->freq)
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

// Function to print the Trie_Node list
void print_trie_nodes(Trie_Node *head) {
    while (head != NULL) {
        printf("Character: %c, Frequency: %d\n", head->ch, head->freq);
        head = head->sibling;
    }
}

// Test function
void test_sorting() {
    // Create test nodes with different frequencies
    Trie_Node *node1 = create_node('a', 5);
    Trie_Node *node2 = create_node('b', 3);
    Trie_Node *node3 = create_node('c', 8);
    Trie_Node *node4 = create_node('d', 2);
    Trie_Node *node5 = create_node('e', 6);

    // Link the nodes into a linked list
    node1->sibling = node2;
    node2->sibling = node3;
    node3->sibling = node4;
    node4->sibling = node5;

    // Print the original list
    printf("Original list:\n");
    print_trie_nodes(node1);

    // Sort the list by frequency
    Trie_Node *sorted_nodes = msort(node1);

    // Print the sorted list
    printf("\nSorted list by frequency:\n");
    print_trie_nodes(sorted_nodes);
}

int main() {
    test_sorting();  // Run the test
    return 0;
}