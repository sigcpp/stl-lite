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
* Intialize tester and start a specific set of unit tests
*/

#include <iostream>
#include <string>
#include <array>

#include "tester.h"

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

int main()
{
   //TODO: configure the tester based on cmd-line

   std::array<int, 0> a;

   try
   {
      std::cout << "Running tests: "; //TODO: conditionally print: which destination?
      runTests();
   }
   catch (const std::string& msg)
   {
      std::cout << msg << '\n'; //TODO: print in appropriate stream based in cmd-line
   }
   //TODO handle other exceptions

   //TODO: conditionally print summary to appropriate destination
   summarizeTests();
}