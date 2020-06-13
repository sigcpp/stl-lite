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
* Define a collection of test suites and functions to manage the collection
* Edit this file with a lot of care: many carefully designed macros to make suite definition easy and safe
* Go to the section at the end of this file to add a test suite
*/

#include <string>
#include <unordered_map>

#include "suites.h"

//keyed collection of all test suites defined 
static std::unordered_map<std::string, suite_runner_type> test_suites;

//macros to facilitate 1-step definition and addition of suite runners to suites collection
//TEST_SUITE is the only macro necessary, but the other macros are defined and used so that
//the section where test suites are added looks declarative and cohesive

#define DECLARE_BUILD_SUITES_COLLECTION void build_suites_collection();

#define BUILD_SUITES_COLLECTION \
	if (collection_not_built) \
		build_suites_collection();

#define START_SUITES_COLLECTION \
void build_suites_collection() \
{

#define END_SUITES_COLLECTION \
}


//macro to declare a suite runner function and to add it to the suites collection
//Example: TEST_SUITE(array_test) is to add the following two lines of code:
// void array_test();
// test_suites["array_test"] = array_test;
#define TEST_SUITE(SUITE_NAME) \
	void SUITE_NAME(); \
	test_suites[#SUITE_NAME] = SUITE_NAME;

//flag to denote if suites collection is already built
static bool collection_not_built{ true };

//provide read-only access to collection of all test suites defined
//build collection "just in time" if it has not already been built
const std::unordered_map<std::string, suite_runner_type>& get_test_suites()
{
	DECLARE_BUILD_SUITES_COLLECTION; // void build_suites_collection();
	BUILD_SUITES_COLLECTION;		 // if (collection_not_built) build_suites_collection();

	collection_not_built = false;
	return test_suites;
}


//this section is intentionally placed at the end of file to make it easier and safer to add test suites
//the collection is as good as being built with a initializer-list ctor, but the technique used here provides
//a 1-step approach instead of the 2-step approach that would be necessary if initializer-list ctor is used
//(2 steps: declare suite runner and add suite runner to the collection)

START_SUITES_COLLECTION //same as void build_suites_collection() {  

	//add one line per test suite: macro parameter should be the name of a suite runner function
	//the semi-colon at the end of macro invocation is not required but its use make things look authentic

	TEST_SUITE(array_test);

// do not add/edit anything after this line
END_SUITES_COLLECTION	// same as }
