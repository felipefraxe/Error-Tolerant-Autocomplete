#include "vecTrie.hpp"

VectorTrie::VectorTrie(unsigned int size)
{
    this->size = size;
    this->root = make_unique<Node>(size);
}

void VectorTrie::insert(const std::vector<unsigned int>& vec, unsigned int id)
{
    Node *curr = this->root.get();
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (curr->children[vec[i]] == nullptr)
            curr->children[vec[i]] = make_unique<Node>(this->size);
        curr = curr->children[vec[i]].get();
    }
    curr->id = id;
    curr->is_terminal = true;
}

int VectorTrie::get(vector<unsigned int>& vec)
{
    Node *curr = this->root.get();
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (curr->children[vec[i]] == nullptr)
            return -1;
        curr = curr->children[vec[i]].get();
    }
    return curr->is_terminal ? curr->id : -1;
}
