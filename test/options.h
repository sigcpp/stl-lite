/*
* tester.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained. 
* - Attribution may be augmented to include additional authors 
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Declare testing infrastructure
*/

#ifndef STL_LITE_OPTIONS_H
#define STL_LITE_OPTIONS_H

#include <iostream>
#include <string>

#include "tester.h"

struct Options {
   bool header{ true };
   bool summary{ true };
   std::string header_text{ "Running $exe" };
   passReportMode prm;
   unsigned short fail_threshold = 0;
   fileOpenMode fileOpenMode{ fileOpenMode::noFile };
   std::string output_filename;
   std::string command_name;
};


Options get_options(char* arguments[], const std::size_t size);

void apply_options(Options options, std::ofstream& fileOut);

passReportMode get_pass_report_mode(const std::string_view& value, bool fileOutput);

fileOpenMode get_file_open_mode(const std::string_view& value);

unsigned short get_fail_threshold(const std::string_view& sv);

void replace_all(std::string& str, const std::string& substr, const std::string& new_substr);

#endif