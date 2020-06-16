/*
* driver-test.cpp
* Nick DeMarco
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Tester for test-stl-lite driver
*/

#include "verifiers.h"
#include "options.h"
#include "tester.h"
#include "utils.h"

#include <iostream>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <tuple>

void test_get_options();

void driver_test()
{
	test_get_options();
}

//this fn is not in use right now, still deciding whether I want to use a function to create expected options
//or just explicitly set the expected options
Options create_expected_options(std::filesystem::path output_filepath, std::string command_name,
	std::string suites_to_run, bool header = true, bool summary = true,
	std::string header_text = "Running $suite", pass_report_mode prm = pass_report_mode::indicate,
	unsigned short fail_threshold = 0, file_open_mode fom = file_open_mode::no_file)
{
	Options e;

	e.output_filepath = output_filepath;
	e.command_name = command_name;
	e.suites_to_run = suites_to_run;
	e.header = header;
	e.summary = summary;
	e.header_text = header_text;
	e.prm = prm;
	e.fail_threshold = fail_threshold;
	e.fom = fom;

	return e;
}

void compare(const Options& received, const Options& expected, const std::string& test_case)
{
	is_true(received.header == expected.header, (test_case + " header").data());
	is_true(received.summary == expected.summary, (test_case + " summary").data());
	is_true(received.header_text == expected.header_text, (test_case + " header text").data());
	is_true(received.prm == expected.prm, (test_case + " pass report mode").data());
	is_true(received.fail_threshold == expected.fail_threshold, (test_case + " fail threshold").data());
	is_true(received.fom == expected.fom, (test_case + " file open mode").data());
	is_true(received.output_filepath == expected.output_filepath, (test_case + " output filepath").data());
	is_true(received.command_name == expected.command_name, (test_case + " command name").data());
	is_true(received.suites_to_run == expected.suites_to_run, (test_case + " suites to run").data());
}

std::vector<std::pair<std::string, Options>> generate_expected_options()
{
	std::vector<std::pair<std::string, Options>> args_and_options;
	Options o;

	std::string cmd_line_0("C:/Libraries/stl-lite/array_test.exe");
	o.command_name = "array_test";
	o.header_text = "Running $suites";
	args_and_options.push_back({ cmd_line_0, o });

	std::string cmd_line_1("C:/Libraries/stl-lite/array_test.exe -h no");
	o.header = false;
	args_and_options.push_back({ cmd_line_1, o });

	std::string cmd_line_2("C:/Libraries/stl-lite/array_test.exe -h yes");
	o.header = true;
	args_and_options.push_back({ cmd_line_2, o });

	std::string cmd_line_3("C:/Libraries/stl-lite/array_test.exe -p none");
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_3, o });

	std::string cmd_line_4("C:/Libraries/stl-lite/array_test.exe -p indicate");
	o.prm = pass_report_mode::indicate;
	args_and_options.push_back({ cmd_line_4, o });

	std::string cmd_line_5("C:/Libraries/stl-lite/array_test.exe -p detail");
	o.prm = pass_report_mode::detail;
	args_and_options.push_back({ cmd_line_5, o });

	return args_and_options;
}

void test_get_options()
{
	//array with only path to test the behavior of default options
	char* args[100];

	std::vector<std::pair<std::string, Options>> args_and_options = generate_expected_options();
	for (auto iter = args_and_options.begin(); iter != args_and_options.end(); ++iter)
	{
		std::size_t cmd_line_number{ 0 };
		std::vector<std::string> v = split(iter->first, ' ');
		for (std::size_t i = 0; i < v.size(); ++i)
			args[i] = v[i].data();

		Options o = get_options(args, v.size());

		compare(o, iter->second, "cmd_line_" + std::to_string(cmd_line_number));
	}
}