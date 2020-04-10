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
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <cstring>
#include <string>
#include <string_view>
#include <cstddef>
#include <sstream>
#include <regex>

#include "tester.h"

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

bool fileExists(const char* fileName);

void processArguments(std::vector<std::string_view> arguments,
    bool& printHeader, bool& printSummary, std::ostream& out);

int main(int argc, char* argv[])
{
    std::vector<std::string_view> arguments;
    for (int i = 0; i < argc; ++i)
        arguments.emplace_back(argv[i]);

    bool printHeader{ true };
    bool printSummary{ true };
    char header[512]{ "Running " };
    strcat_s(header, argv[0]);
    std::ostream& out = std::cout;

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


void processArguments(std::vector<std::string_view> arguments,
    bool& printHeader, bool& printSummary, std::ostream& out)
{
    for (std::size_t i = 1; i < arguments.size(); i+=2)
    {
        if (arguments[i] == "-h")
        {
            if (arguments[i + 1] == "yes")
                printHeader = true;
            else if (arguments[i + 1] == "no")
                printHeader = false;
        }
        
        else if (arguments[i] == "-ht")
        {
            //process text
        }

        else if (arguments[i] == "-p")
        {
            if (arguments[i + 1] == "detail")
                setPassReportMode(passReportMode::detail);
            else if (arguments[i + 1] == "indicate")
                setPassReportMode(passReportMode::indicate);
            else if (arguments[i + 1] == "none")
                setPassReportMode(passReportMode::none);
            else if (arguments[i + 1] == "auto")
            {
                for (std::size_t i = 1; i < arguments.size(); i += 2)
                {
                    if (arguments[i] == "-f" || arguments[i] == "-fo"
                        || arguments[i] == "-fa")
                        setPassReportMode(passReportMode::none);
                    else
                        setPassReportMode(passReportMode::indicate);
                }
            }
        }
        else if (arguments[i] == "-s")
        {
            if (arguments[i + 1] == "yes")
                printSummary = true;
            else if (arguments[i + 1] == "no")
                printSummary = false;
        }
        else if (arguments[i] == "-t")
        {
            //need to convert arguments[i + 1] from string_view to unsigned
            //and supply that argument to setFailThreshold() function
        }
        else if (arguments[i] == "-f")
        {
            //convert filename to c-string
            //check if filename has a file extension, if it doesn't, append .out
            //test if file "arguments[i + 1]" exists, if it does print error message
            //else construct an ofstream that writes to filename
        }
        else if (arguments[i] == "-fo")
        {

        }
        else if (arguments[i] == "-fa")
        {

        }
    }
}

bool fileExists(const char* fileName)
{
    struct stat buffer;
    return (stat(fileName, &buffer) == true);
}
