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

//must be defined in a unit-specific source file such as "array-test.cpp"
void runTests();

void processCmdLine(char* arguments[], const std::size_t size);
passReportMode getPassReportMode(const std::string_view& value);

void replace_all(std::string& str, const std::string& substr, 
   const std::string& new_substr);

int main(int argc, char* argv[])
{
    processCmdLine(argv, argc);

   return getTestsFailed();
}

void processCmdLine(char* arguments[], const std::size_t size)
{
   using std::string;
   using std::string_view;
   using std::size_t;
   
   //default values for cmd-line option names in name-value pairs
   bool printHeader{ true };
   bool printSummary{ true };
   string headerText("Running $exe");
   string_view passReportMode("auto");
   unsigned short failThreshold(0);
   string_view fileOpenMode;
   string outputFilename;

   //names in name-value pair for cmd-line options
   constexpr string_view option_name_header{ "-h" }, option_name_header_text{ "-ht" },
      option_name_summary{ "-s" }, option_name_prm{ "-p" }, option_name_file{ "-f" },
      option_name_file_overwrite{ "-fo" }, option_name_file_append{ "-fa" },
      option_name_threshold{ "-t" };

   //values in name-value pair for boolean cmd-line options
   constexpr string_view option_value_yes{ "yes" };

   //value for auto prm 
   constexpr string_view option_value_auto{ "auto" };

   //begin parsing cmd-line arguments
   for (size_t i = 1; i < size; i += 2)
   {
      string_view name(arguments[i]), value(arguments[i + 1]);
      
      if (name == option_name_header)
         printHeader = (value == option_value_yes);
      else if (name == option_name_header_text)
         headerText = value;
      else if (name == option_name_prm)
         passReportMode = value;
      else if (name == option_name_summary)
         printSummary = (value == option_value_yes);
      else if (name == option_name_threshold)
      {
         std::from_chars(value.data(), value.data() + value.size(), failThreshold);
         setFailThreshold(failThreshold);
      }
      else if (name == option_name_file || name == option_name_file_overwrite ||
         name == option_name_file_append)
      {
         fileOpenMode = name;
         outputFilename = value;
      }
   } //done parsing cmd-line arguments

   //apply pass report mode, translating "auto" based on output means
   if (passReportMode == option_value_auto)
   {
      if (!fileOpenMode.empty())
         setPassReportMode(passReportMode::none);
      else
         setPassReportMode(passReportMode::indicate);
   }
   else
      setPassReportMode(getPassReportMode(passReportMode));

   //extract the exe name without path or filename extension
   //the result is used to expand the macro $exe
   std::filesystem::path exePath(arguments[0]);
   string filenameNoExt = exePath.replace_extension("").filename().string();

   //replace $exe macro in header text only if a header text was defined
   //and the print header option is enabled
   const string exeMacro{ "$exe" };
   if (printHeader && !headerText.empty())
      replace_all(headerText, exeMacro, filenameNoExt);
   else
      headerText = "";

   setHeaderText(headerText);

   //if a filename was supplied, replace $exe macro in filename
   if (!outputFilename.empty())
      replace_all(outputFilename, exeMacro, filenameNoExt);

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
void replace_all(std::string& str, const std::string& substr, 
   const std::string& new_substr)
{
  auto pos = str.find(substr);
  auto substr_size = substr.size();
   for (; pos != std::string::npos; pos = str.find(substr, pos + substr_size))
      str.replace(pos, substr_size, new_substr);
}


//convert text version of pass report mode to enum equivalent
passReportMode getPassReportMode(const std::string_view& value)
{
   constexpr std::string_view value_none{ "none" }, 
                              value_indicate{ "indicate" },
                              value_detail{ "detail" };

   if (value == value_none)
      return passReportMode::none;
   else if (value == value_indicate)
      return passReportMode::indicate;
   if (value == value_detail)
      return passReportMode::detail;
   else
   {
      assert(false);

      //TO DO: review exception mgmt in the entire file; using temp value
      throw "invalid value for pass report mode";
   }
}

