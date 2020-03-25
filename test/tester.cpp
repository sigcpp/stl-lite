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
#include <exception>
#include <sstream>
#include <climits>

#include "tester.h"

static passReportMode passMode { passReportMode::indicate };
void setPassReportMode(passReportMode mode)
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


//counter for number of tests run, failed
static unsigned testsDone;
static unsigned testsFailed;

bool lastOutputEndedInLineBreak{ false };

//track number of tests and check test result
void assert(bool success, const char* hint)
{
   ++testsDone;
   std::ostringstream message;

   lastOutputEndedInLineBreak = false;
   if (success)
   {
      if (passMode == passReportMode::indicate)
         message << '.';
      else if (passMode == passReportMode::detail)
      {
         if (testsDone == 1)
            message << '\n';
         message << "Test# " << testsDone << ": Pass (" << hint << ")\n";
         lastOutputEndedInLineBreak = true;
      }
   }
   else
   {
      ++testsFailed;

      if (passMode != passReportMode::detail)
         message << '\n';
      message << "Test# " << testsDone << ": FAIL (" << hint << ")\n";
      lastOutputEndedInLineBreak = true;

      if (testsFailed > failThreshold)
         throw message.str();
   }

   std::cout << message.str();
}


//print a simple test report
void summarizeTests()
{
   //TODO: there should always be one and exactly one empty line before summary
   //-assume if an exception was thrown earlier on test failure, the client
   //-printed the msg and caused a line break after printing the msg
   if (!lastOutputEndedInLineBreak)
      std::cout << '\n';
   else if (testsFailed <= failThreshold)
      std::cout << '\n';

   std::cout << "Tests completed: " << testsDone << '\n';
   std::cout << "Tests passed: " << testsDone - testsFailed << '\n';
   std::cout << "Tests failed: " << testsFailed << '\n';

   if (testsFailed > failThreshold)
      std::cout << "Tests stopped after " << testsFailed << " failure(s)\n";
}