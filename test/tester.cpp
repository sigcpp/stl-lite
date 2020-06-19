/*
* tester.cpp
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Define testing infrastructure
*/

#include <iostream>
#include <sstream>
#include <string>
#include <climits>

#include "utils.h"
#include "tester.h"
#include "verifier-exceptions.h"


static std::string headerText("Running $suite:");
void set_header_text(std::string text)
{
	headerText = text;
}


static pass_report_mode prm{ pass_report_mode::indicate };
void set_pass_report_mode(pass_report_mode mode)
{
	prm = mode;
}


static int fail_threshold{ 0 };
void set_fail_threshold(int value)
{
	fail_threshold = value;
}


void set_max_fail_threshold()
{
	fail_threshold = INT_MAX;
}


static std::ostream* pOut{ &std::cout };
void set_output(std::ostream& o)
{
	pOut = &o;
}


void log(const char* s)
{
	*pOut << s;
}


void log_line(const char* s)
{
	*pOut << s << '\n';
}


int tests_done_total;
int tests_done_suite;
int tests_failed_total;
int tests_failed_suite;

int get_tests_failed_total()
{
	return tests_failed_total;
}


static std::string suite_name;
static unsigned suites_run;
void start_suite(const std::string& name)
{
	suite_name = name;
	++suites_run;

	//print a separator between test suites
	if (tests_done_suite != 0)
		*pOut << "\n\n";

	tests_done_suite = 0;
	tests_failed_suite = 0;

	//print header text after expanding macro $suite
	if (!headerText.empty()) {
		const std::string suite_macro{ "$suite" };
		std::string suite_header{ headerText };
		replace_all(suite_header, "$suite", name);
		*pOut << suite_header << ":\n";
	}
}


//helper for formatting output
bool last_output_ended_in_linebreak{ false };


//print a report for the suite
void summarize_suite()
{
	if (!last_output_ended_in_linebreak)
		*pOut << "\n\n";

	*pOut << "Tests completed: " << tests_done_suite << '\n';
	*pOut << "Tests passed: " << tests_done_suite - tests_failed_suite << '\n';
	*pOut << "Tests failed: " << tests_failed_suite << '\n';

	last_output_ended_in_linebreak = true;
}


//print a report across all suites
void summarize_tests()
{
	//TODO: there should always be one and exactly one empty line before summary
	//-assume if an exception was thrown earlier on test failure, the client
	//-printed the msg and caused a line break after printing the msg
	if (!last_output_ended_in_linebreak)
		*pOut << '\n';
	else if (tests_failed_total <= fail_threshold)
		*pOut << '\n';

	*pOut << "Suites run: " << suites_run << '\n';
	*pOut << "Tests completed: " << tests_done_total << '\n';
	*pOut << "Tests passed: " << tests_done_total - tests_failed_total << '\n';
	*pOut << "Tests failed: " << tests_failed_total << '\n';

	if (tests_failed_total > fail_threshold)
		*pOut << "Tests stopped after " << tests_failed_total << " failure(s)\n";
}


//track number of tests and check test result
void verify(bool success, const char* hint)
{
	++tests_done_total;
	++tests_done_suite;

	//assume stream is at start of line on first call 
	last_output_ended_in_linebreak = tests_done_suite == 1;

	std::ostringstream message;

	if (success) {
		if (prm == pass_report_mode::indicate)
			message << '.';
		else if (prm == pass_report_mode::detail) {
			message << "Test# " << tests_done_suite << ": Pass (" << hint << ")\n";
			last_output_ended_in_linebreak = true;
		}
	}
	else {
		++tests_failed_total;
		++tests_failed_suite;

		if (!last_output_ended_in_linebreak)
			message << '\n';
		message << "Test# " << tests_done_suite << ": FAIL (" << hint << ")\n";
		last_output_ended_in_linebreak = true;

		if (tests_failed_total > fail_threshold) {
			*pOut << message.str();
			throw fail_threshold_met_error();
		}
	}

	*pOut << message.str();
}
