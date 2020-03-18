/*
* tester.cpp
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice shown on lines 3 and 4 must be retained.
* That information may be relocated but be conspicuous in all derived work.
*
* Provide testing infrastructure
*/

#include <iostream>
#include <exception>
#include <sstream>

//counter for number of tests run
static unsigned testCount;

//track number of tests and check test result
void assert(bool success, const char* hint)
{
   testCount++;

   if (success)
      std::cout << '.';
   else
   {
      std::ostringstream message;
      message << "Failed Test# " << testCount << "; Hint: " << hint;
      throw message.str();
   }
}

//print a simple test report
void summarizeTests()
{
   std::cout << "\nTests successful (" << testCount << " tests)\n";
}