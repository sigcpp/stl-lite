/*
* utils.cpp
* Sean Murthy, Nick DeMarco
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Define general-purpose utility functions and such
*/

#include <string>
#include <string_view>

#include "utils.h"

//replace all instances of a substring with a new substring
void replace_all(std::string& str, const std::string& substr, const std::string& new_substr)
{
	auto pos = str.find(substr);
	auto substr_size = substr.size(), new_substr_size = new_substr.size();
	for (; pos != std::string::npos; pos = str.find(substr, pos + new_substr_size))
		str.replace(pos, substr_size, new_substr);
}
