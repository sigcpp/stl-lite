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

#include "tester.h"
#include "options.h"
#include "options-exceptions.h"

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

void show_error(const char* message);
void show_usage();
void show_error_and_usage(const char* message);

int main(int argc, char* argv[])
{
	Options options;
	std::ofstream fileOut;

	try {
		options = get_options(argv, argc);
		apply_options(options, fileOut);
	}
	catch (const cmd_line_error& cle) {
		show_error_and_usage(cle.what());
		return -1;
	}
	catch (const file_error& fe) {
		show_error_and_usage(fe.what());
		return -2;
	}
	catch (const std::exception& e) {
		show_error((std::string{ "Unexpected error: " } +e.what()).data());
		return -3;
	}
	catch (...) {
		show_error("Unexpected and untyped error");
		return -4;
	}


	try {
		runTests();
	}
	catch (const std::string& msg) {
		logLine(msg.data());
		if (options.fom == file_open_mode::no_file)
			show_error(msg.data());
	}

	if (options.summary)
		summarizeTests();

	return getTestsFailed();
}


void show_error_and_usage(const char* message)
{
	show_error(message);
	std::cout << '\n';
	show_usage();
}


void show_error(const char* message)
{
	std::cout << message << '\n';
}


void show_usage()
{
	std::cout << "usage: [command <option>]" << '\n'
		<< "       [-h <yes, no>] [-ht <header text>] [-p <none, indicate, detail>]" << '\n'
		<< "       [-s <yes, no>] [-t <number of failed tests to tolerate>] [-f <output file name>]" << '\n'
		<< "       [-fo <file name>] [-fa <file name>] ......" << '\n'
		<< "For more details, see "
		<< "https://github.com/sigcpp/stl-lite/wiki/Command-line-interface-for-the-test-driver#options\n";
}