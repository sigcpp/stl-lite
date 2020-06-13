/*
* suites.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Declare types and functions to access collection of test suites
*/

#ifndef STL_LITE_SUITES_H
#define STL_LITE_SUITES_H

#include <string>
#include <unordered_map>

using suite_runner_type = void(*)();
const std::unordered_map<std::string, suite_runner_type>& get_test_suites();

#endif
