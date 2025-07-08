#include <algorithm>

#include "bitmap.hpp"

using std::min;

Bitmap::Bitmap(string_view str, unordered_set<char>& charset, unsigned int offset)
{
    for (char ch : charset)
        this->map[ch] = vector<bool>(str.length() + offset, 0);

    for (size_t i = offset; i < str.length() + offset; i++)
    {
        char ch = str[i - offset];
        if (this->map.contains(ch))
            this->map[ch][i] = 1;
    }
}

unsigned int Bitmap::extract_bitmask(char ch, unsigned int lo, unsigned int offset)
{
    if (!this->map.contains(ch))
        return 0;

    vector<bool> bitset = this->map[ch];    
    unsigned int bitset_len = bitset.size();
    if (lo >= bitset_len)
        return 0;

    unsigned int bitmask = 0;
    unsigned int bit_idx = offset - 1;
    unsigned int n = min(lo + offset, bitset_len);
    while (lo < n)
    {
        if (bitset[lo] == 1)
            bitmask |= (1 << bit_idx);
        lo++;
        bit_idx--;
    }
    return bitmask;
}
