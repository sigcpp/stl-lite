/*
* options.cpp
* Sean Murthy, Nick DeMarco
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Tester options
*/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <charconv>
#include <string>
#include <string_view>
#include <cstddef>
#include <cassert>

#include "options.h"

Options get_options(char* arguments[], const std::size_t size)
{
   using std::string;
   using std::string_view;
   using std::size_t;

   Options options;

   //names in name-value pair for cmd-line options
   constexpr string_view option_name_header{ "-h" }, option_name_header_text{ "-ht" },
      option_name_summary{ "-s" }, option_name_prm{ "-p" }, option_name_file{ "-f" },
      option_name_file_overwrite{ "-fo" }, option_name_file_append{ "-fa" },
      option_name_threshold{ "-t" };

   //values in name-value pair for boolean cmd-line options
   constexpr string_view option_value_yes{ "yes" };
   std::string_view prm;

   //begin parsing arguments from index 1 because args[0] is command name
   for (size_t i = 1; i < size; i += 2)
   {
      string_view name(arguments[i]), value(arguments[i + 1]);

      if (name == option_name_header)
         options.header = (value == option_value_yes);
      else if (name == option_name_header_text)
         options.header_text = value;
      else if (name == option_name_prm)
         prm = value; //delay converting to prm until after file openmode is known
      else if (name == option_name_summary)
         options.summary = (value == option_value_yes);
      else if (name == option_name_threshold)
         std::from_chars(value.data(), value.data() + value.size(), options.fail_threshold);
      else if (name == option_name_file || name == option_name_file_overwrite ||
         name == option_name_file_append)
      {
         options.fileOpenMode = name;
         options.output_filename = value;
      }
   }

   options.prm = get_pass_report_mode(prm, !options.fileOpenMode.empty());

   //extract "command name" from the first "argument"
   //command_name is just exe filename without path or extension
   std::filesystem::path exePath = arguments[0];
   options.command_name = exePath.replace_extension("").filename().string();

   return options;
}


//TODO: Remove the call to runTests() and summarizeTests() in this fn and explicitly call them in main()
void apply_options(Options options)
{
   //replace $exe macro in header text only if a header text was defined
   //and the print header option is enabled
   const std::string exeMacro{ "$exe" };
   if (options.header && !options.header_text.empty())
      replace_all(options.header_text, exeMacro, options.command_name);
   else
      options.header_text = "";

   setHeaderText(options.header_text);

   //if a filename was supplied, replace $exe macro in filename
   if (!options.output_filename.empty())
      replace_all(options.output_filename, exeMacro, options.command_name);

   //the file stream must be alive when runTests() is called
   std::ofstream fileOut;
   //if output to file option not enabled, use standard output
   if (options.output_filename.empty())
      setOutput(std::cout);
   else
   {
      setOutput(fileOut);

      std::filesystem::path fileOutPath = options.output_filename;
      //if not extension supplied, append the default ".out" extension
      if (fileOutPath.extension() == "")
         fileOutPath.replace_extension(".out");
      //if -f option enabled, make sure the file doesn't already exist
      std::string_view option_name_file("-f");
      if (options.fileOpenMode == option_name_file && std::filesystem::exists(fileOutPath))
      {
         std::cerr << "Output file already exists";
         return;
      }
      //if -fa option is enabled, set open mode to append
      using std::ios;
      std::string_view option_name_file_append("-fa");
      fileOut.open(fileOutPath, options.fileOpenMode == option_name_file_append ? ios::app : ios::out);
      //check for errors opening file
      if (!fileOut.is_open())
      {
         std::cerr << "Error opening output file";
         return;
      }
   }
}

//replace all instances of a substring with a new substring
void replace_all(std::string& str, const std::string& substr, const std::string& new_substr)
{
   auto pos = str.find(substr);
   auto substr_size = substr.size(), new_substr_size = new_substr.size();
   for (; pos != std::string::npos; pos = str.find(substr, pos + new_substr_size))
      str.replace(pos, substr_size, new_substr);
}


//convert text version of pass report mode to enum equivalent
//treats empty value as "auto" 
//the flag fileOutput is used only if value is "auto" (or empty)
passReportMode get_pass_report_mode(const std::string_view& value, bool fileOutput)
{
   constexpr std::string_view value_none{ "none" }, value_indicate{ "indicate" },
      value_detail{ "detail" }, value_auto{ "auto" };

   if (value == value_none)
      return passReportMode::none;
   else if (value == value_indicate)
      return passReportMode::indicate;
   if (value == value_detail)
      return passReportMode::detail;
   if (value == value_auto || value.empty())
      return fileOutput ? passReportMode::none : passReportMode::indicate;
   else
   {
      assert(false);

      //TO DO: review exception mgmt in the entire file; using temp value for now
      throw "invalid value for pass report mode";
   }
}