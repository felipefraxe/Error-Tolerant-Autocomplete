#include <fstream>
#include <iostream>
#include <unordered_set>
#include <print>

#include "bitmap.hpp"
#include "eva.hpp"
#include "trie.hpp"
#include "utils.hpp"

using std::getline;
using std::ifstream;
using std::unordered_set;
using std::string;
using std::string_view;
using std::print;
using std::println;


#define QUERY_LIMIT 2048


vector<string> process(string_view query, unordered_set<char>& charset, EditVectorAutomata& eva, Trie& trie);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        println("Usage: {} <suggestions>", argv[0]);
        return 1;
    }

    Trie trie = Trie();
    unordered_set<char> charset;

    ifstream file;
    file.open(argv[1], ifstream::in);
    for (string line; getline(file, line);)
    {
        for (string str : split(line, ' '))
        {
            string normalized_str = normalize(str);
            for (char ch : normalized_str)
                charset.insert(ch);
            trie.insert(normalized_str);
        }
    }
    file.close();

    const unsigned int edit_distance_threshold = 1;
    EditVectorAutomata eva(edit_distance_threshold);

    print("Query: ");
    for (string query; getline(std::cin, query) && !query.empty();)
    {
        query = normalize(query);

        vector<string> suggestions = process(query, charset, eva, trie);
        for (string_view suggestion : suggestions)
            println("Suggestion: {}", suggestion);
        println("{}", suggestions.size());

        print("Query: ");
    }

    return 0;
}

vector<string> process(string_view query, unordered_set<char>& charset, EditVectorAutomata& eva, Trie& trie)
{
    if (query.length() <= eva.edit_distance_threshold || query.length() > QUERY_LIMIT)
        return {};

    Bitmap bitmap = Bitmap(query, charset, eva.edit_distance_threshold);
    vector<string> suggestions = trie.traverse(query, eva, bitmap);
    return suggestions;
}
