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


void replace_all(std::string& str, const std::string& substr, const std::string& new_substr);

#endif