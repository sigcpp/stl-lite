/*
* main.cpp
* Sean Murthy
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
#include <string>
#include <array>
#include <vector>
#include <string_view>
#include <cstddef>

#include "tester.h"

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

int main(int argc, char* argv[])
{
    std::vector<std::string_view> arguments;
    for (int i = 0; i < argc; ++i)
        arguments.emplace_back(argv[i]);

   try
   {
      std::cout << "Running tests:\n"; //TODO: conditionally print: which destination?
      runTests();
   }
   catch (const std::string& msg)
   {
      std::cout << msg << '\n'; //TODO: print in appropriate stream based in cmd-line
   }
   //TODO handle other exceptions

   //TODO: conditionally print summary to appropriate destination
   summarizeTests();

   return getTestsFailed();
}