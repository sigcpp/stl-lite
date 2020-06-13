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
#include <stdexcept>

using suite_runner_type = void(*)();
using suites_map_type = std::unordered_map<std::string, suite_runner_type>;

const suites_map_type& get_test_suites();


//error when adding a test suite to collection of suites
class test_suite_add_error : public std::runtime_error {

public:
	test_suite_add_error(const std::string& base, const std::string& name)
		: std::runtime_error{ base + ": " + name } {}

};


#endif
