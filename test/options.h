/*
* options.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Declare tester options
*/

#ifndef STL_LITE_OPTIONS_H
#define STL_LITE_OPTIONS_H

#include <filesystem>
#include <string>

#include "tester.h"

enum class file_open_mode { no_file, new_file, overwrite, append };

struct Options {
	bool header{ true };
	bool summary{ true };
	std::string header_text{ "Running $suite" };
	pass_report_mode prm{ pass_report_mode::indicate };
	unsigned short fail_threshold = 0;
	file_open_mode fom{ file_open_mode::no_file };
	std::filesystem::path output_filepath;
	std::string command_name;
	std::string suites;
};


Options get_options(char* arguments[], const std::size_t size);

void apply_options(const Options& options, std::ofstream& fileOut);

pass_report_mode get_pass_report_mode(const std::string_view& value, file_open_mode fom);

file_open_mode get_file_open_mode(const std::string_view& name);

unsigned short get_fail_threshold(const std::string_view& value);

bool strtobool(const std::string_view& value);

#endif