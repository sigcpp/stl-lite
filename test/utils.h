/*
* utils.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Declare generanl-purpose utility functions and such
*/

#ifndef STL_LITE_UTILS_H
#define STL_LITE_UTILS_H

#include <string>
#include <vector>

void replace_all(std::string& str, const std::string& substr, const std::string& new_substr);

//split a delimited string or string_view into vector
template<typename T>
std::vector<T> split(const T& s, char delimiter)
{
    static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>,
        "requires std::string type or std::string_view type");

    std::vector<T> result;
    typename T::size_type pos, last_pos{ 0 };
    while ((pos = s.find(delimiter, last_pos)) != T::npos) {
        if (s[last_pos] != delimiter)
            result.emplace_back(s.substr(last_pos, pos - last_pos));
        last_pos = pos + 1;
    }

    if (last_pos < s.size())
        result.emplace_back(s.substr(last_pos, pos - last_pos));

    return result;
}

#endif