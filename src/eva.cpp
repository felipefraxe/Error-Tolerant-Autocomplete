#include <algorithm>
#include <bitset>
#include <queue>
#include <print>
#include <memory>

#include "eva.hpp"


using std::queue;
using std::make_unique;
using std::min;
using std::unique_ptr;


EditVectorAutomata::EditVectorAutomata(unsigned int edit_distance_threshold)
{
    this->edit_distance_threshold = edit_distance_threshold;
    this->editvec_length = (2 * edit_distance_threshold) + 1;
    const unsigned int TAU = edit_distance_threshold + 1;

    vector<unsigned int> terminal(editvec_length, TAU);
    vector<unsigned int> initial(editvec_length, 0);
    for (unsigned int i = 0; i < editvec_length; i++)
        initial[i] = abs_diff(i, edit_distance_threshold);

    this->state_to_editvec.reserve(pow(2, editvec_length));
    this->state_to_editvec.push_back(terminal);
    this->state_to_editvec.push_back(initial);

    VectorTrie editvector_to_state = VectorTrie(TAU + 1);
    editvector_to_state.insert(terminal, 0);
    editvector_to_state.insert(initial, 1);

    queue<unsigned int> state_queue;
    state_queue.push(1);
    while (state_queue.empty() == false)
    {
        unsigned int curr_state = state_queue.front();
        state_queue.pop();
        if (curr_state == 0)
            continue;

        vector<unsigned int> editvec = this->state_to_editvec[curr_state];
        for (unsigned int bitmask = 0, n = pow(2, this->editvec_length); bitmask < n; bitmask++)
        {
            vector<unsigned int> next_editvec = this->compute_next_editvec(editvec, bitmask);
            int next_state = editvector_to_state.get(next_editvec);
            if (next_state == -1)
            {
                next_state = this->state_to_editvec.size();
                this->state_to_editvec.push_back(next_editvec);
                state_queue.push(next_state);
                editvector_to_state.insert(next_editvec, next_state);
            }

            this->transitions[{curr_state, bitmask}] = next_state;
        }
    }
}

vector<unsigned int> EditVectorAutomata::compute_next_editvec(vector<unsigned int>& editvec, unsigned int bitmask)
{
    const unsigned int TAU = this->edit_distance_threshold + 1;

    vector<unsigned int> next_editvec(editvec.size(), 0);
    for (unsigned int i = 0; i < editvec.size(); i++)
    {
        unsigned int deletion = i > 0 ? next_editvec[i - 1] : TAU;
        unsigned int insertion = i < editvec.size() - 1 ? editvec[i + 1] : TAU;
        unsigned int substitution = editvec[i];

        unsigned int idx = (editvec.size() - 1) - i;
        bool match = ((bitmask >> idx) & 1) == 1;

        next_editvec[i] = match ? editvec[i] : min({deletion, insertion, substitution}) + 1;
        next_editvec[i] = min(next_editvec[i], TAU);
    }

    return next_editvec;
}

unsigned int EditVectorAutomata::get_next_state(unsigned int state, unsigned int bitmask)
{
    
    if (this->transitions.contains({state, bitmask}) == false)
        throw std::out_of_range("Transition not found for state and bitmask in EditVectorAutomata");
    return this->transitions[{state, bitmask}];
}

vector<unsigned int> EditVectorAutomata::get_editvec(unsigned int state)
{
    if (state >= this->state_to_editvec.size())
        throw std::out_of_range("State index out of range in EditVectorAutomata");
    return this->state_to_editvec[state];
}

void EditVectorAutomata::print_automata(void)
{
    std::println("Transitions: ");
    for (auto [key, next_state] : this->transitions)
    {
        auto [state, bitmask] = key;
        std::println("State: {} {} Bitmask: {} -> Next State: {}",
            state, this->get_editvec(state), std::bitset<3>(bitmask).to_string(), this->get_editvec(next_state));
    }
}
