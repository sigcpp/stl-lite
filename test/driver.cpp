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
#include <sstream>
#include <cassert>

#include "utils.h"
#include "suites.h"
#include "tester.h"
#include "options.h"
#include "options-exceptions.h"
#include "verifier-exceptions.h"


//error codes returned back from main are negative
enum class driver_error_code {
	no_error = 0,

	//anticipated errors in getting and applying options
	cmd_line_options = -1, file_options = -2,

	//unanticipated errors in getting and applying options
	unexpected_typed_options = -51, unexpected_untyped_options = -52,

	//anticipated in run_suites, but before any test is actually run
	cmd_line_run_suites = -101, test_suite_add = -101,

	//anticipated in run_suites, likely at least one test may have been done
	file_run_suites = -121, fail_threshold_met = -122,

	//anticipated in run_suites, not known if a test may have been run
	unexpected_typed_run_suites = -171, unexpected_untyped_run_suites = -172
};


void run_suites(const Options& options);
static std::string format_error_message(const char* message, driver_error_code ec);
static int show_error(const char* message, driver_error_code ec);
static void show_usage(const char* program_path);
static int show_error_and_usage(const char* message, const char* program_path, driver_error_code ec);


//return  < 0: error; including fail threshold met
//return == 0: no error, no tests failed
//return  > 0: no error, some tests failed but fail threshold not met; number of failed tests returned
int main(int argc, char* argv[])
{
	Options options;
	std::ofstream fileOut;

	try {
		options = get_options(argv, argc);
		apply_options(options, fileOut);
	}
	catch (const cmd_line_error& cle) {
		return show_error_and_usage(cle.what(), argv[0], driver_error_code::cmd_line_options);
	}
	catch (const file_error& fe) {
		return show_error_and_usage(fe.what(), argv[0], driver_error_code::file_options);
	}
	catch (const std::exception& e) {
		const auto message = format_message("Unexpected error", e.what());
		return show_error(message.data(), driver_error_code::unexpected_typed_options);
	}
	catch (...) {
		return show_error("Unexpected error", driver_error_code::unexpected_untyped_options);
	}

	//no errors in getting and applying options: OK to run test suites

	driver_error_code error_code{ driver_error_code::no_error };
	std::string message;
	try {
		run_suites(options);
	}

	//errors where it is certain no tests have yet been run
	catch (const cmd_line_error& cle) {
		return show_error_and_usage(cle.what(), argv[0], driver_error_code::cmd_line_run_suites);
	}
	catch (const test_suite_add_error& tae) {
		return show_error(tae.what(), driver_error_code::test_suite_add);
	}

	//errors where one or more tests have likely been run
	catch (const fail_threshold_met_error& fte) {
		error_code = driver_error_code::fail_threshold_met;
		message = fte.what();
	}

	//errors where we do not know if tests have been run
	catch (const std::exception& e) {
		error_code = driver_error_code::unexpected_typed_run_suites;
		message = format_message("Unexpected error", e.what());
	}
	catch (...) {
		error_code = driver_error_code::unexpected_untyped_run_suites;
		message = "Unexpected error";
	}

	//log message to output destination, and also to screen if output is file
	if (error_code != driver_error_code::no_error) {
		message = '\n' + format_error_message(message.data(), error_code) + '\n';
		log_line(message.data());
		if (options.fom != file_open_mode::no_file)
			std::cout << message << '\n'; //not calling show_error because message is already formatted 
	}

	//print summary because one or more tests may have been run
	if (options.summary)
		summarize_tests();

	//return totaly tests failed only if there was no error: the caller should know if there was an error
	if (error_code == driver_error_code::no_error)
		return get_tests_failed_total();
	else
		return static_cast<int>(error_code);
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

static int show_error_and_usage(const char* message, const char* program_path, driver_error_code ec)
{
	show_error(message, ec);
	std::cout << '\n';
	show_usage(program_path);

	return static_cast<int>(ec);
}


static std::string format_error_message(const char* message, driver_error_code ec)
{
	std::stringstream result;
	result << "Error " << static_cast<int>(ec) << "; " << message;
	return result.str();
}


static int show_error(const char* message, driver_error_code ec)
{
	std::cout << format_error_message(message, ec) << '\n';
	return static_cast<int>(ec);
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