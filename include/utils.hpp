#ifndef UTILS_HPP
#define UTILS_HPP

#include <functional>
#include <utility>
#include <string>
#include <vector>

using std::hash;
using std::pair;
using std::string;
using std::string_view;
using std::vector;


struct pair_hash {
    size_t operator()(const pair<unsigned int, unsigned int>& p) const
    {
        size_t seed = 0;
        seed ^= hash<unsigned int>{}(p.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<unsigned int>{}(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

unsigned int abs_diff(unsigned int num1, unsigned int num2);
char ascii_fold(int ch);
string normalize(string_view data);
unsigned int pow(unsigned int base, unsigned int exp);
vector<string> split(string_view str, char delim);

#endif