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
#include <filesystem>
#include <charconv>
#include <string>
#include <string_view>
#include <cstddef>
#include <cassert>

#include "tester.h"
#include "options.h"

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

int main(int argc, char* argv[])
{
   Options options = get_options(argv, argc);

   apply_options(options);

   try
   {
       runTests();
   }
   catch (const std::string& msg)
   {
       logLine(msg.data());
   }

   if (options.summary)
       summarizeTests();

   return getTestsFailed();
}