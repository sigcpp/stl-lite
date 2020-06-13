/*
* suites.cpp
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Define functions to manage collection of test suites
*/

#include <string>
#include <unordered_map>

#include "suites.h"

//all test suites defined
static std::unordered_map<std::string, suite_runner_type> test_suites;

//flag to denote if suites collection is already built
static bool collection_built{ true };


void build_suites_collection();

//provide read-only access to collection of all test suites defined
//build collection "just in time" if it has not already been built
const std::unordered_map<std::string, suite_runner_type>& get_test_suites()
{
	if (collection_built)
		build_suites_collection();
	
	collection_built = false;
	return test_suites;
}


//this function is intentionally placed at the end of file to make it easier and safer to add test suites
//the collection is as good as being built with a list initializer ctor, but the technique used here makes it
//easier to add suites
void build_suites_collection()
{
	void array_test();
	test_suites["array_test"] = array_test;
}
