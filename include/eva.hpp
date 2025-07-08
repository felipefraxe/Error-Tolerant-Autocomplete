#ifndef EVA_HPP
#define EVA_HPP


#include <unordered_map>
#include <vector>

#include "utils.hpp"
#include "vecTrie.hpp"


using std::hash;
using std::pair;
using std::unordered_map;
using std::vector;


class EditVectorAutomata
{
public:
    EditVectorAutomata(unsigned int edit_distance_threshold);

    unsigned int edit_distance_threshold;
    unsigned int editvec_length;
    unsigned int get_next_state(unsigned int state, unsigned int editvec);
    vector<unsigned int> get_editvec(unsigned int state);
    void print_automata(void);

private:
    unordered_map<pair<unsigned int, unsigned int>, unsigned int, pair_hash> transitions;
    vector<vector<unsigned int>> state_to_editvec;
    vector<unsigned int> compute_next_editvec(vector<unsigned int>& editvec, unsigned int bitmask);
};

#endif