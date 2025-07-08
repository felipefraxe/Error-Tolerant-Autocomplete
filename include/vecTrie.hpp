#ifndef VECTRIE_HPP
#define VECTRIE_HPP

#include <memory>
#include <vector>

using std::make_unique;
using std::unique_ptr;
using std::vector;

class VectorTrie {
public:
    VectorTrie(unsigned int size);
    void insert(const std::vector<unsigned int>& vec, unsigned int id);
    int get(vector<unsigned int>& vec);
private:
    struct Node {
        Node(unsigned int size)
        {
            this->children.resize(size);
        }
        unsigned int id = 0;
        bool is_terminal = false;
        vector<unique_ptr<Node>> children;
    };
    unsigned int size;
    unique_ptr<Node> root;
};

#endif