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
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <cstddef>

#include "tester.h"

using std::string;
using std::string_view;
using std::ios;

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

void processCmdLine(const std::vector<string_view>& arguments);

int main(int argc, char* argv[])
{
    std::vector<string_view> arguments;
    for (int i = 0; i < argc; ++i)
        arguments.emplace_back(argv[i]);
    
    processCmdLine(arguments);

   return getTestsFailed();
}

static const string defaultHeaderText("Running $exe");

void processCmdLine(const std::vector<string_view>& arguments)
{
   bool printHeader{ true };
   bool printSummary{ true };
   string headerText(defaultHeaderText);
   string_view passReportMode("auto");
   unsigned short failThreshold(0);
   string_view fileOpenMode;
   string outputFilename;

   // begin parsing cmd-line arguments
   for (size_t i = 1; i < arguments.size(); i += 2)
   {
      if (arguments[i] == "-h")
         printHeader = arguments[i + 1] != "no";
      else if (arguments[i] == "-ht")
         headerText = arguments[i + 1];
      else if (arguments[i] == "-p")
         passReportMode = arguments[i + 1];
      else if (arguments[i] == "-s")
         printSummary = arguments[i + 1] != "no";
      else if (arguments[i] == "-t")
      {
         const string_view& v = arguments[i + 1];
         std::from_chars(v.data(), v.data() + v.size(), failThreshold);
         setFailThreshold(failThreshold);
      }
      else if (arguments[i] == "-f" || arguments[i] == "-fo" || arguments[i] == "-fa")
      {
         fileOpenMode = arguments[i];
         outputFilename = arguments[i + 1];
      }
   } //done parsing cmd-line arguments

   //set pass report mode to the proper value
   if (passReportMode == "detail")
       setPassReportMode(passReportMode::detail);
   else if (passReportMode == "indicate")
       setPassReportMode(passReportMode::indicate);
   else if (passReportMode == "none")
       setPassReportMode(passReportMode::none);
   else if (passReportMode == "auto")
   {
      //if output to file option enabled
      if (!fileOpenMode.empty())
         setPassReportMode(passReportMode::none);
      else
         setPassReportMode(passReportMode::indicate);
   }

   //extract the exe name without path or filename extension
   //the result is used to expand the macro $exe
   std::filesystem::path exePath(arguments[0]);
   string filenameNoExt = exePath.replace_extension("").filename().string();

   //replace $exe macro in header text only if a header text was defined
   //and the print header option is enabled
   if (printHeader && !headerText.empty())
   {
      string::size_type pos = headerText.find("$exe");
      for (; pos != string::npos; pos = headerText.find("$exe"))
         headerText.replace(pos, 4, filenameNoExt);
   }
   //else do not print a header text
   else
      headerText = "";

   setHeaderText(headerText);

   //if a filename was supplied, replace $exe macro in filename
   if (!outputFilename.empty())
   {
      string::size_type pos = outputFilename.find("$exe");
      for (; pos != string::npos; pos = outputFilename.find("$exe"))
         outputFilename.replace(pos, 4, filenameNoExt);
   }

   // the file stream must be alive when runTests() is called
   std::ofstream fileOut;
   //if output to file option not enabled, use standard output
   if (outputFilename.empty())
      setOutput(std::cout);
   else
   {
      setOutput(fileOut);

      std::filesystem::path fileOutPath = outputFilename;
      //if not extension supplied, append the default ".out" extension
      if (fileOutPath.extension() == "")
         fileOutPath.replace_extension(".out");
      //if -f option enabled, make sure the file doesn't already exist
      if (fileOpenMode == "-f" && std::filesystem::exists(fileOutPath))
      {
         std::cerr << "Output file alredy exists";
         return;
      }

      //if -fa option is enabled, set open mode to append
      fileOut.open(fileOutPath, fileOpenMode == "-fa" ? ios::app : ios::out);
      //check for errors opening file
      if (!fileOut.is_open())
      {
         std::cerr << "Error opening output file";
         return;
		}
   }

   try
   {
      runTests();
   }
   catch (const std::string & msg)
   {
      std::cout << msg << '\n'; //TODO: print in appropriate stream based in cmd-line
   }

   //if print summary option is enabled
   if (printSummary)
      summarizeTests();
}