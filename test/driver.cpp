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
#include <map>

#include "tester.h"
#include "options.h"
#include "options-exceptions.h"

void run_suites(const Options& options);
static void show_error(const char* message);
static void show_usage(const char* program_path);
static void show_error_and_usage(const char* message, const char* program_path);

int main(int argc, char* argv[])
{
	Options options;
	std::ofstream fileOut;

	try {
		options = get_options(argv, argc);
		apply_options(options, fileOut);
	}
	catch (const cmd_line_error& cle) {
		show_error_and_usage(cle.what(), argv[0]);
		return -1;
	}
	catch (const file_error& fe) {
		show_error_and_usage(fe.what(), argv[0]);
		return -2;
	}
	catch (const std::exception& e) {
		show_error((std::string{ "Unexpected error: " } +e.what()).data());
		return -3;
	}
	catch (...) {
		show_error("Unexpected error");
		return -4;
	}


	try {
		run_suites(options);
	}
	catch (const std::string& msg) {
		log_line(msg.data());
		if (options.fom == file_open_mode::no_file)
			show_error(msg.data());
	}

	if (options.summary)
		summarize_tests();

	return get_tests_failed_total();
}


//run test suites
void runTests();
void run_suites(const Options& options)
{
	using suite_runner_type = void(*)();
	std::map<std::string, suite_runner_type> suites{
		//add one entry for each test suite to run
		//suite-name and suite-runner, both in braces
		{"array-test", runTests}
	};

	auto size = suites.size();
	for (auto suite : suites) {
		start_suite(suite.first);
		suite.second();
		if (size > 1 && options.summary)
			summarize_suite();
	}
}


//the following functions assume they are called only from main so that the parameters are always correct
//assertion and error handling are included by design

static void show_error_and_usage(const char* message, const char* program_path)
{
	show_error(message);
	std::cout << '\n';
	show_usage(program_path);
}


static void show_error(const char* message)
{
	std::cout << message << '\n';
}


static void show_usage(const char* program_path)
{
	std::string program_filename = std::filesystem::path{ program_path }.filename().string();

	std::cout << "Usage: " << program_filename << " {option_name option_value}\n\n";

	std::cout <<
		"  * Options are specified as name-value pairs, with at least one space between name and value.\n"
		"  * Every option name begins with a - (hyphen), and every option name should have a value.\n"
		"  * Any number of options may be specified. If an option repeats, the latest occurrence is used.\n";

	std::cout <<
		"\nThe following options are available. "
		"Angle brackets are placeholders for option values:\n\n";

	std::cout <<
		"  -h  <yes, no>\n"
		"  -ht <header text>\n"
		"  -s  <yes, no>\n"
		"  -p  <detail, indicate, none, auto>\n"
		"  -t  <fail threshold>\n"
		"  -fn <output file path>\n"
		"  -fo <output file path>\n"
		"  -fa <output file path>\n"
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