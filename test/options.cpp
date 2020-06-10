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
* Cmd-line options
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
#include "options-exceptions.h"

Options get_options(char* arguments[], const std::size_t size)
{
	//args should not be empty: args[0] should be path to executable file ("command name")
	//number of args should be odd: command name followed by name-value pairs for options
	assert(size != 0);
	assert(size % 2 == 1);

	if (size == 0)
		throw invalid_cmd_line{ "empty command line" };

	if (size % 2 == 0)
		throw invalid_cmd_line{ "incorrect number of options" };

	Options options;

	//extract "command name" from first arg: command name is just command filename without path or extension
	//command file path cannot be empty
	std::filesystem::path command_path{ arguments[0] };
	assert(!command_path.empty());
	if (command_path.empty())
		throw invalid_cmd_line{ "missing command filepath" };

	options.command_name = command_path.replace_extension("").filename().string();

	//names in name-value pair for cmd-line options
	constexpr std::string_view option_name_header{ "-h" }, option_name_header_text{ "-ht" },
		option_name_summary{ "-s" }, option_name_prm{ "-p" }, option_name_threshold{ "-t" },
		option_name_file_start{ "-f" };

	std::string_view prm_value;
	std::string output_filepath_value;

	//begin parsing arguments from index 1 because args[0] corresponds to command name
	for (std::size_t i = 1; i < size; i += 2) {
		std::string_view name{ arguments[i] }, value{ arguments[i + 1] };

		//option name or value cannot be empty; options names must begin with a - 
		assert(!name.empty());
		assert(!value.empty());
		assert(name[0] == '-');

		if (name.empty())
			throw invalid_option_name{ "empty option name" };
		else if (value.empty())
			throw invalid_option_value{ "empty option value" };
		else if (name[0] != '-')
			throw invalid_option_name{ name };

		if (name == option_name_header)
			options.header = strtobool(value);
		else if (name == option_name_header_text)
			options.header_text = value;
		else if (name == option_name_prm)
			prm_value = value; //delay converting prm to enum until after file open mode is known
		else if (name == option_name_summary)
			options.summary = strtobool(value);
		else if (name == option_name_threshold) {
			if (value.length() < 6 && std::all_of(value.begin(), value.end(), std::isdigit))
				options.fail_threshold = get_fail_threshold(value);
			else {
				assert(false);
				throw invalid_option_value{ value };
			}
		}
		else if (name._Starts_with(option_name_file_start)) {
			options.fom = get_file_open_mode(name);
			output_filepath_value = value;
		}
		else { //unknown option
			assert(false);
			throw invalid_option_name{ name };
		}
	}

	options.prm = get_pass_report_mode(prm_value, options.fom);

	//replace $cmd macro with command name in header text
	const std::string cmd_macro{ "$cmd" };
	if (options.header && !options.header_text.empty())
		replace_all(options.header_text, cmd_macro, options.command_name);
	else
		options.header_text = "";

	//replace $cmd macro in output file path; also set file extension to ".out" if extension missing
	if (options.fom != file_open_mode::no_file) {
		replace_all(output_filepath_value, cmd_macro, options.command_name);

		options.output_filepath = output_filepath_value;
		if (options.output_filepath.extension().empty())
			options.output_filepath.replace_extension(".out");
	}

	return options;
}


void apply_options(const Options& options, std::ofstream& fileOut)
{
	setHeaderText(options.header_text);
	setPassReportMode(options.prm);
	setFailThreshold(options.fail_threshold);

	//if output to file option not enabled, use standard output
	//else open output file in appropriate mode
	if (options.fom == file_open_mode::no_file)
		setOutput(std::cout);
	else {
		setOutput(fileOut);

		//enforce create-only file open mode
		if (options.fom == file_open_mode::new_file && std::filesystem::exists(options.output_filepath)) {
			assert(false);
			throw file_error{ "Output file already exists", options.output_filepath };
		}

		//open file in append mode or overwrite mode: create-only mode has already been checked
		using std::ios;
		fileOut.open(options.output_filepath, options.fom == file_open_mode::append ? ios::app : ios::out);

		if (!fileOut.is_open()) {
			assert(false);
			throw file_error{ "Error opening output file", options.output_filepath };
		}
	}
}


//convert text version of pass report mode to enum equivalent
//treats empty value as "auto" 
//the flag fileOutput is used only if value is "auto" (or empty)
pass_report_mode get_pass_report_mode(const std::string_view& value, file_open_mode fom)
{
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
	else {
		assert(false);
		throw invalid_option_value{ value };
	}
}


file_open_mode get_file_open_mode(const std::string_view& name)
{
	constexpr std::string_view option_name_file_new("-fn"), option_name_file_overwrite("-fo"),
		option_name_file_append("-fa");

	if (name == option_name_file_new)
		return file_open_mode::new_file;
	if (name == option_name_file_overwrite)
		return file_open_mode::overwrite;
	if (name == option_name_file_append)
		return file_open_mode::append;
	else {
		assert(false);
		throw invalid_option_name{ name };
	}
}


//convert text version of Boolean value
bool strtobool(const std::string_view& value)
{
	constexpr std::string_view value_yes{ "yes" }, value_no{ "no" };

	if (value == value_yes)
		return true;
	if (value == value_no)
		return false;
	else {
		assert(false);
		throw invalid_option_value{ value };
	}
}


unsigned short get_fail_threshold(const std::string_view& sv)
{
	long value;
	std::from_chars(sv.data(), sv.data() + sv.size(), value);
	if (value > USHRT_MAX) {
		assert(false);
		throw invalid_option_value{ sv };
	}
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
