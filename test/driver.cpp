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
using std::size_t;

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

void processCmdLine(char* arguments[], const size_t size);

void replace_all(string& str, const string& substr, 
   const string& toReplace);

int main(int argc, char* argv[])
{
    processCmdLine(argv, argc);

   return getTestsFailed();
}

void processCmdLine(char* arguments[], const size_t size)
{
   bool printHeader{ true };
   bool printSummary{ true };
   string headerText("Running $exe");
   string_view passReportMode("auto");
   unsigned short failThreshold(0);
   string_view fileOpenMode;
   string outputFilename;

   constexpr string_view name_header{ "-h" }, name_header_text{ "-ht" },
      name_summary{ "-s" }, namePRM{ "-p" }, name_file{ "-f" },
      name_file_overwrite{ "-fo" }, name_file_append{ "-fa" },
      name_threshold{ "-t" };

   constexpr string_view value_yes{ "yes" };
   constexpr string_view value_detail{ "detail" },
      value_indicate{ "indicate" }, value_none{ "none" }, value_auto{ "auto" };

   //begin parsing cmd-line arguments
   for (size_t i = 1; i < size; i += 2)
   {
      string_view name(arguments[i]), value(arguments[i + 1]);
      
      if (name == name_header)
         printHeader = (value == value_yes);
      else if (name == name_header_text)
         headerText = value;
      else if (name == namePRM)
         passReportMode = value;
      else if (name == name_summary)
         printSummary = (value == value_yes);
      else if (name == name_threshold)
      {
         const string_view& v = arguments[i + 1];
         std::from_chars(v.data(), v.data() + v.size(), failThreshold);
         setFailThreshold(failThreshold);
      }
      else if (name == name_file || name == name_file_overwrite ||
         name == name_file_append)
      {
         fileOpenMode = name;
         outputFilename = value;
      }
   } //done parsing cmd-line arguments

   //set pass report mode to the proper value
   if (passReportMode == value_detail)
       setPassReportMode(passReportMode::detail);
   else if (passReportMode == value_indicate)
       setPassReportMode(passReportMode::indicate);
   else if (passReportMode == value_none)
       setPassReportMode(passReportMode::none);
   else if (passReportMode == value_auto)
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
   const string exeMacro{ "$exe" };
   if (printHeader && !headerText.empty())
   {
      replace_all(headerText, exeMacro, filenameNoExt);
   }
   else
      headerText = "";

   setHeaderText(headerText);

   //if a filename was supplied, replace $exe macro in filename
   if (!outputFilename.empty())
   {
      replace_all(outputFilename, exeMacro, filenameNoExt);
   }

   //the file stream must be alive when runTests() is called
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
      if (fileOpenMode == name_file && std::filesystem::exists(fileOutPath))
      {
         std::cerr << "Output file alredy exists";
         return;
      }
      //if -fa option is enabled, set open mode to append
      using std::ios;
      fileOut.open(fileOutPath, fileOpenMode == name_file_append ? ios::app : ios::out);
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
      (outputFilename.empty() ? std::cout : fileOut )<< msg << '\n';
   }

   //if print summary option is enabled
   if (printSummary)
      summarizeTests();
}

//Replace all instances of a substring with a different substring
void replace_all(string& str, const string& substr, 
   const string& new_substr)
{
   string::size_type pos = str.find(substr);
   for (; pos != string::npos; pos = str.find(substr, pos + substr.size()))
      str.replace(pos, 4, new_substr);
}