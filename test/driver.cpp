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

void replace_all(std::string& str, const std::string& substr, 
   const std::string& new_substr);

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

   constexpr string_view option_name_header{ "-h" }, option_name_header_text{ "-ht" },
      option_name_summary{ "-s" }, option_name_prm{ "-p" }, option_name_file{ "-f" },
      option_name_file_overwrite{ "-fo" }, option_name_file_append{ "-fa" },
      option_name_threshold{ "-t" };

   constexpr string_view option_value_yes{ "yes" };
   constexpr string_view option_value_detail{ "detail" },
      option_value_indicate{ "indicate" }, option_value_none{ "none" },
      option_value_auto{ "auto" };

   //begin parsing cmd-line arguments
   for (size_t i = 1; i < size; i += 2)
   {
      string_view option_name(arguments[i]), value(arguments[i + 1]);
      
      if (option_name == option_name_header)
         printHeader = (value == option_value_yes);
      else if (option_name == option_name_header_text)
         headerText = value;
      else if (option_name == option_name_prm)
         passReportMode = value;
      else if (option_name == option_name_summary)
         printSummary = (value == option_value_yes);
      else if (option_name == option_name_threshold)
      {
         std::from_chars(value.data(), value.data() + value.size(), failThreshold);
         setFailThreshold(failThreshold);
      }
      else if (option_name == option_name_file || option_name == option_name_file_overwrite ||
         option_name == option_name_file_append)
      {
         fileOpenMode = option_name;
         outputFilename = value;
      }
   } //done parsing cmd-line arguments

   //set pass report mode to the proper value
   if (passReportMode == option_value_detail)
       setPassReportMode(passReportMode::detail);
   else if (passReportMode == option_value_indicate)
       setPassReportMode(passReportMode::indicate);
   else if (passReportMode == option_value_none)
       setPassReportMode(passReportMode::none);
   else if (passReportMode == option_value_auto)
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
      if (fileOpenMode == option_name_file && std::filesystem::exists(fileOutPath))
      {
         std::cerr << "Output file alredy exists";
         return;
      }
      //if -fa option is enabled, set open mode to append
      using std::ios;
      fileOut.open(fileOutPath, fileOpenMode == option_name_file_append ? ios::app : ios::out);
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
