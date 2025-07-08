#include "utils.hpp"

unsigned int abs_diff(unsigned int num1, unsigned int num2)
{
    return num1 > num2 ? num1 - num2 : num2 - num1;
}

unsigned int pow(unsigned int base, unsigned int exp)
{
    unsigned int result = 1;
    while (exp > 0)
    {
        if (exp % 2 != 0)
            result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

vector<string> split(string_view str, char delim)
{
    vector<string> tokens;
    tokens.reserve(str.length() / 8);

    size_t lo = 0;
    size_t hi;
    while ((hi = str.find(delim, lo)) != string::npos)
    {
        if (hi > lo)
            tokens.emplace_back(str.substr(lo, hi - lo));
        lo = hi + 1;
    }

    if (lo < str.length())
        tokens.emplace_back(str.substr(lo));

    return tokens;
}

char ascii_fold(int ch)
{
    switch (ch)
    {
        case -96: case -95: case -94: case -93: case -92:
        case -128: case -127: case -126: case -125: case -124:
            return 'a';
        case -88: case -87: case -86: case -85:
        case -120: case -119: case -118: case -117:
            return 'e';
        case -84: case -83: case -82: case -81:
        case -116: case -115: case -114: case -113:
            return 'i';
        case -78: case -77: case -76: case -75: case -74:
        case -110: case -109: case -108: case -107: case -106:
            return 'o';
        case -71: case -70: case -69: case -68:
        case -103: case -102: case -101: case -100:
            return 'u';
        case -89: case -121:
            return 'c';
        case -79: case -111:
            return 'n';
        default:
            return '?';
    }
}

string normalize(string_view data)
{
    string normalized = "";
    normalized.reserve(data.length());

    for (int ch : data)
    {
        if (ch == -61)
            continue;

        if (ch < 0 || ch >= 128)
            ch = ascii_fold(ch);
        else
            ch = tolower(ch);

        normalized += ch;
    }

    return normalized;
}
