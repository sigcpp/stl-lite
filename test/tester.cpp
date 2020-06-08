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
void setHeaderText(std::string text)
{
	headerText = text;
}


static pass_report_mode passMode{ pass_report_mode::indicate };
void setPassReportMode(pass_report_mode mode)
{
	passMode = mode;
}


static unsigned short failThreshold{ 0 };
void setFailThreshold(unsigned short value)
{
	failThreshold = value;
}


void setMaxFailThreshold()
{
	failThreshold = USHRT_MAX;
}


static std::ostream* pOut{ &std::cout };
void setOutput(std::ostream& o)
{
	pOut = &o;
}


//number of tests run and failed
static unsigned testsDone;
unsigned getTestsDone()
{
	return testsDone;
}


static unsigned testsFailed;
unsigned getTestsFailed()
{
	return testsFailed;
}


bool lastOutputEndedInLineBreak{ false };

//track number of tests and check test result
void verify(bool success, const char* hint)
{
	++testsDone;
	if (testsDone == 1 && !headerText.empty())
		*pOut << headerText << ":\n";

	std::ostringstream message;

	//assume stream is at start of line on first call 
	lastOutputEndedInLineBreak = testsDone == 1;

	if (success) {
		if (passMode == pass_report_mode::indicate)
			message << '.';
		else if (passMode == pass_report_mode::detail) {
			message << "Test# " << testsDone << ": Pass (" << hint << ")\n";
			lastOutputEndedInLineBreak = true;
		}
	}
	else {
		++testsFailed;

		if (!lastOutputEndedInLineBreak)
			message << '\n';
		message << "Test# " << testsDone << ": FAIL (" << hint << ")\n";
		lastOutputEndedInLineBreak = true;

		if (testsFailed > failThreshold)
			throw message.str();
	}

	*pOut << message.str();
}


//print a simple test report
void summarizeTests()
{
	//TODO: there should always be one and exactly one empty line before summary
	//-assume if an exception was thrown earlier on test failure, the client
	//-printed the msg and caused a line break after printing the msg
	if (!lastOutputEndedInLineBreak)
		*pOut << '\n';
	else if (testsFailed <= failThreshold)
		*pOut << '\n';

	*pOut << "Tests completed: " << testsDone << '\n';
	*pOut << "Tests passed: " << testsDone - testsFailed << '\n';
	*pOut << "Tests failed: " << testsFailed << '\n';

	if (testsFailed > failThreshold)
		*pOut << "Tests stopped after " << testsFailed << " failure(s)\n";
}


void log(const char* s)
{
	*pOut << s;
}


void logLine(const char* s)
{
	*pOut << s << '\n';
}
