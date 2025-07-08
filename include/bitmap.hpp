#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string_view;
using std::unordered_map;
using std::unordered_set;
using std::vector;


class Bitmap {
public:
    Bitmap(string_view str, unordered_set<char>& charset, unsigned int threshold);
    unsigned int extract_bitmask(char ch, unsigned int lo, unsigned int hi);

private:
    unordered_map<char, vector<bool>> map;
};

#endif