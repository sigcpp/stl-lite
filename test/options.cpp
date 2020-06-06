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
       option_name_summary{ "-s" }, option_name_prm{ "-p" }, option_name_threshold{ "-t" },
       option_name_file_start{ "-f" };

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
         prm = value; //delay converting prm to enum until after file open mode is known
      else if (name == option_name_summary)
         options.summary = (value == option_value_yes);
      else if (name == option_name_threshold)
          options.fail_threshold = get_fail_threshold(value);
      else if (name._Starts_with(option_name_file_start))
      {
         options.fom = get_file_open_mode(name);
         options.output_filename = value;
      }
   }

   options.prm = get_pass_report_mode(prm, options.fom);

   //extract "command name" from the first "argument"
   //command_name is just exe filename without path or extension
   std::filesystem::path exePath = arguments[0];
   options.command_name = exePath.replace_extension("").filename().string();

   return options;
}


//TODO: Remove the call to runTests() and summarizeTests() in this fn and explicitly call them in main()
void apply_options(Options options, std::ofstream& fileOut)
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

   //if output to file option not enabled, use standard output
   if (options.fom == file_open_mode::no_file && options.output_filename.empty())
      setOutput(std::cout);
   else
   {
      setOutput(fileOut);

      std::filesystem::path fileOutPath = options.output_filename;
      //if not extension supplied, append the default ".out" extension
      if (fileOutPath.extension() == "")
         fileOutPath.replace_extension(".out");
      //if -f option enabled, make sure the file doesn't already exist
      if (options.fom == file_open_mode::create && std::filesystem::exists(fileOutPath))
      {
         std::cerr << "Output file already exists";
         return;
      }
      //if -fa option is enabled, set open mode to append
      using std::ios;
      fileOut.open(fileOutPath, options.fom == file_open_mode::append ? ios::app : ios::out);
      //check for errors opening file
      if (!fileOut.is_open())
      {
         std::cerr << "Error opening output file";
         return;
      }
   }
}


//convert text version of pass report mode to enum equivalent
//treats empty value as "auto" 
//the flag fileOutput is used only if value is "auto" (or empty)
pass_report_mode get_pass_report_mode(const std::string_view& value, file_open_mode fom) {
    constexpr std::string_view value_none{ "none" }, value_indicate{ "indicate" },
        value_detail{ "detail" }, value_auto{ "auto" };

    if (value == value_none)
        return pass_report_mode::none;
    else if (value == value_indicate)
        return pass_report_mode::indicate;
    if (value == value_detail)
        return pass_report_mode::detail;
    if (value == value_auto || value.empty())
        return fom == file_open_mode::no_file ? pass_report_mode::indicate : pass_report_mode::none;
    else
    {
        assert(false);

        //TO DO: review exception mgmt in the entire file; using temp value for now
        throw "invalid value for pass report mode";
    }
}

file_open_mode get_file_open_mode(const std::string_view& name) {
    constexpr std::string_view option_name_file("-f"), option_name_file_overwrite("-fo"),
        option_name_file_append("-fa");

    if (name == option_name_file)
        return file_open_mode::create;
    if (name == option_name_file_overwrite)
        return file_open_mode::overwrite;
    if (name == option_name_file_append)
        return file_open_mode::append;
    else
    {
        assert(false);

        //TO DO: review exception mgmt in the entire file; using temp value for now
        throw "invalid value for file open mode";
    }
}


unsigned short get_fail_threshold(const std::string_view& sv) {
    long value;
    std::from_chars(sv.data(), sv.data() + sv.size(), value);
    return static_cast<unsigned short>(value);
}


//replace all instances of a substring with a new substring
void replace_all(std::string& str, const std::string& substr, const std::string& new_substr)
{
   auto pos = str.find(substr);
   auto substr_size = substr.size(), new_substr_size = new_substr.size();
   for (; pos != std::string::npos; pos = str.find(substr, pos + new_substr_size))
      str.replace(pos, substr_size, new_substr);
}
