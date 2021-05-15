#ifndef RESCOM_STRINGS_HPP
#define RESCOM_STRINGS_HPP
#include <string>

inline void replaceAll(std::string& str, char toReplace, char replacement)
{
    for (auto pos = str.find(toReplace); pos != std::string::npos; pos = str.find(toReplace, pos + 1))
    {
        str[pos] = replacement;
    }
}

inline void toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::toupper(c); });
}

inline std::string toUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::toupper(c); });

    return str;
}

#endif //RESCOM_STRINGS_HPP
