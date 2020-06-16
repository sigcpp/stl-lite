/*
* driver.cpp
* Sean Murthy, Nick DeMarco
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Intialize tester and start unit tests
*/

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <filesystem>
#include <unordered_map>
#include <algorithm>
#include <cassert>

#include "utils.h"
#include "suites.h"
#include "tester.h"
#include "options.h"
#include "options-exceptions.h"

//error codes returned back from main are negative
enum class error_code {
	no_error = 0,
	cmd_line_initial = -1, file_initial = -2, 
	unexpected_typed_initial = -51, unexpected_untyped_initial = -52,
	cmd_line_run = -100, file_run = -101, suite_add_run = -102, fail_threshold_run = -103,
	unexpected_typed_run = -151, unexpected_untyped_run = -152
};


void run_suites(const Options& options);
static int show_error(const char* message, error_code ec);
static void show_usage(const char* program_path);
static int show_error_and_usage(const char* message, const char* program_path, error_code ec);


//return  < 0: error
//return == 0: no error, no tests failed
//return  > 0: no error, tests failed; number of failed tests returned
int main(int argc, char* argv[])
{
	Options options;
	std::ofstream fileOut;

	try {
		options = get_options(argv, argc);
		apply_options(options, fileOut);
	}
	catch (const cmd_line_error& cle) {
		return show_error_and_usage(cle.what(), argv[0], error_code::cmd_line_initial);
	}
	catch (const file_error& fe) {
		return show_error_and_usage(fe.what(), argv[0], error_code::file_initial);
	}
	catch (const std::exception& e) {
		auto message = format_message("Unexpected error", e.what());
		return show_error(message.data(), error_code::unexpected_typed_initial);
	}
	catch (...) {
		show_error("Unexpected error", error_code::unexpected_untyped_initial);
	}


	try {
		run_suites(options);
	}
	catch (const cmd_line_error& cle) {
		show_error_and_usage(cle.what(), argv[0], error_code::cmd_line_run);
	}
	catch (const test_suite_add_error& tae) {
		show_error(tae.what(), error_code::suite_add_run);
	}
	catch (const std::exception& e) {
		auto message = format_message("Unexpected error", e.what());
		show_error(message.data(), error_code::unexpected_typed_run);
	}
	catch (...) {
		show_error("Unexpected error", error_code::unexpected_untyped_run);
	}

	if (options.summary)
		summarize_tests();

	return get_tests_failed_total();
}


//run test suites in sequence: runs all suites defined or only those whose names are specified in options
void run_suites(const Options& options)
{
	//retrieve all test suites defined
	auto suites = get_test_suites();

	//build a collection of suite names specified in the options structucre
	//options.suites_to_run is empty or a semi-colon delimited list of suite names
	const auto names_to_run = split(options.suites_to_run, ';');
	auto run_all_suites = names_to_run.empty();

	//check that the suite names specified in options correspond to suites defined
	//this check is not required to run the suites, but is included to inform the user of any issues
	//silently ignoring an unfound suite leaves the user unaware of the reason the suite doesn't run
	if (!run_all_suites) {
		auto end_suites = suites.cend();
		for (auto& suite_name : names_to_run) {
			if (suites.find(suite_name) == end_suites) {
				assert(false);
				throw invalid_option_value{ std::string{"test suite "} +suite_name + " not defined" };
			}
		}
	}

	//run all suites or only the suites indicated in options
	auto size = suites.size();
	auto begin_names_to_run = names_to_run.cbegin(), end_names_to_run = names_to_run.cend();
	for (const auto& suite : suites) {
		const auto& key = suite.first;
		if (run_all_suites || std::find(begin_names_to_run, end_names_to_run, key) != end_names_to_run) {
			start_suite(key);
			suite.second();
			if (size > 1 && options.summary)
				summarize_suite();
		}
	}
}


//the following functions assume they are called only from main so that the parameters are always correct
//assertion and error handling are included by design

static int show_error_and_usage(const char* message, const char* program_path, error_code ec)
{
	show_error(message, ec);
	std::cout << '\n';
	show_usage(program_path);

	return static_cast<int>(ec);
}


static int show_error(const char* message, error_code ec)
{
	int code{ static_cast<int>(ec) };
	std::cout << "Error " << code << "; " << message << '\n';
	return code;
}


static void show_usage(const char* program_path)
{
	auto program_filename = std::filesystem::path{ program_path }.filename().string();

	std::cout << "Usage: " << program_filename << " {option_name option_value}\n\n";

	std::cout <<
		"  * Options are specified as name-value pairs, with at least one space between name and value.\n"
		"  * Every option name begins with a - (hyphen), and every option name should have a value.\n"
		"  * Any number of options may be specified. If an option repeats, the latest occurrence is used.\n";

	std::cout <<
		"\nThe following options are available. "
		"Angle brackets are placeholders for option values:\n\n";

	std::cout <<
		"  -h   <yes, no>\n"
		"  -ht  <header text>\n"
		"  -s   <yes, no>\n"
		"  -p   <detail, indicate, none, auto>\n"
		"  -t   <fail threshold>\n"
		"  -run <semi-colon separated list of suite names>\n"
		"  -fn  <output file path>\n"
		"  -fo  <output file path>\n"
		"  -fa  <output file path>\n"
		"\n";

	std::cout <<
		"Example usages:\n"
		<< "  " << program_filename << " -p indicate -fn results.txt\n"
		<< "  " << program_filename << " -h no -p none -s no\n";

	std::cout <<
		"\nFull documentation at:\n"
		"https://github.com/sigcpp/stl-lite/wiki/Command-line-interface-for-the-test-driver"
		"\n\n";
}