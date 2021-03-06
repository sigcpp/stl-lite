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
static suites_map_type test_suites;

//add an entry to the suites collection, throwing an appropriate exception if insertion fails
static void insert_suite(suites_map_type& suites, const std::string& name, suite_runner_type runner)
{
	if (name.empty())
		throw test_suite_add_error("empty test-suite name", "");
	else if (!suites.insert({ name, runner }).second) {
		if (suites.find(name) == suites.end())
			throw test_suite_add_error("unknown error adding test suite", name);
		else
			throw test_suite_add_error("duplicate test-suite name", name);
	}
}

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
//Example: TEST_SUITE(array_test) adds the following two lines of code:
// void array_test();
// insert_suite(test_suites, "array_test", array_test;
#define TEST_SUITE(SUITE_NAME) \
	void SUITE_NAME(); \
	insert_suite(test_suites, #SUITE_NAME, SUITE_NAME);


//flag to denote if suites collection is already built
static bool collection_not_built{ true };


//provide read-only access to the collection of all test suites defined
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

	//add one line per test suite: macro parameter should be the name of a suite-runner function
	//the semi-colon at the end of macro invocation is not required but its use make things look "authentic"

	TEST_SUITE(array_test);
	TEST_SUITE(driver_test);
	TEST_SUITE(string_view_test);

// do not add/edit anything after this line
END_SUITES_COLLECTION	// same as }
