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

#include "tester.h"

static std::string headerText("Running tests");
void set_header_text(std::string text)
{
	headerText = text;
}


static pass_report_mode prm{ pass_report_mode::indicate };
void set_pass_report_mode(pass_report_mode mode)
{
	prm = mode;
}


static unsigned short fail_threshold{ 0 };
void set_fail_threshold(unsigned short value)
{
	fail_threshold = value;
}


void set_max_fail_threshold()
{
	fail_threshold = USHRT_MAX;
}


static std::ostream* pOut{ &std::cout };
void set_output(std::ostream& o)
{
	pOut = &o;
}


//number of tests run and failed
static unsigned tests_done;
unsigned get_tests_done()
{
	return tests_done;
}


static unsigned tests_failed;
unsigned get_tests_failed()
{
	return tests_failed;
}


bool last_output_ended_in_linebreak{ false };

//track number of tests and check test result
void verify(bool success, const char* hint)
{
	++tests_done;
	if (tests_done == 1 && !headerText.empty())
		*pOut << headerText << ":\n";

	std::ostringstream message;

	//assume stream is at start of line on first call 
	last_output_ended_in_linebreak = tests_done == 1;

	if (success) {
		if (prm == pass_report_mode::indicate)
			message << '.';
		else if (prm == pass_report_mode::detail) {
			message << "Test# " << tests_done << ": Pass (" << hint << ")\n";
			last_output_ended_in_linebreak = true;
		}
	}
	else {
		++tests_failed;

		if (!last_output_ended_in_linebreak)
			message << '\n';
		message << "Test# " << tests_done << ": FAIL (" << hint << ")\n";
		last_output_ended_in_linebreak = true;

		if (tests_failed > fail_threshold)
			throw message.str();
	}

	*pOut << message.str();
}


//print a simple test report
void summarize_tests()
{
	//TODO: there should always be one and exactly one empty line before summary
	//-assume if an exception was thrown earlier on test failure, the client
	//-printed the msg and caused a line break after printing the msg
	if (!last_output_ended_in_linebreak)
		*pOut << '\n';
	else if (tests_failed <= fail_threshold)
		*pOut << '\n';

	*pOut << "Tests completed: " << tests_done << '\n';
	*pOut << "Tests passed: " << tests_done - tests_failed << '\n';
	*pOut << "Tests failed: " << tests_failed << '\n';

	if (tests_failed > fail_threshold)
		*pOut << "Tests stopped after " << tests_failed << " failure(s)\n";
}


void log(const char* s)
{
	*pOut << s;
}


void log_line(const char* s)
{
	*pOut << s << '\n';
}
