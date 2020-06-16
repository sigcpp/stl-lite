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

void test_get_options();

void driver_test()
{
	test_get_options();
}

void reset_defaults(Options& o)
{
	o.header = true;
	o.summary = true;
	o.header_text = "Running $suite";
	pass_report_mode prm = pass_report_mode::indicate;
	o.fail_threshold = 0;
	o.fom = file_open_mode::no_file;
	o.output_filepath = "";
	o.command_name = "array_test";
	o.suites_to_run = "";
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

	using std::string;
	std::string cmd_line_0("C:/Libraries/stl-lite/array_test.exe");
	o.command_name = "array_test";
	args_and_options.push_back({ cmd_line_0, o });
	reset_defaults(o);

	std::string cmd_line_1("C:/Libraries/stl-lite/array_test.exe -h no");
	o.header = false;
	o.header_text = "";
	args_and_options.push_back({ cmd_line_1, o });
	reset_defaults(o);

	std::string cmd_line_2("C:/Libraries/stl-lite/array_test.exe -h yes");
	o.header = true;
	o.header_text = "Running $suite";
	args_and_options.push_back({ cmd_line_2, o });
	reset_defaults(o);

	std::string cmd_line_3("C:/Libraries/stl-lite/array_test.exe -p none");
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_3, o });
	reset_defaults(o);

	std::string cmd_line_4("C:/Libraries/stl-lite/array_test.exe -p indicate");
	o.prm = pass_report_mode::indicate;
	args_and_options.push_back({ cmd_line_4, o });
	reset_defaults(o);

	std::string cmd_line_5("C:/Libraries/stl-lite/array_test.exe -p detail");
	o.prm = pass_report_mode::detail;
	args_and_options.push_back({ cmd_line_5, o });
	reset_defaults(o);

	string cmd_line_6("C:/Libraries/stl-lite/array_test.exe -s no");
	o.summary = false;
	args_and_options.push_back({ cmd_line_6, o });
	reset_defaults(o);

	string cmd_line_7("C:/Libraries/stl-lite/array_test.exe -s yes");
	o.summary = true;
	args_and_options.push_back({ cmd_line_7, o });
	reset_defaults(o);

	string cmd_line_8("C:/Libraries/stl-lite/array_test.exe -t 5");
	o.fail_threshold = 5;
	args_and_options.push_back({ cmd_line_8, o });
	reset_defaults(o);

	string cmd_line_9("C:/Libraries/stl-lite/array_test.exe -h no -ht \"should be ignored\"");
	o.header = false;
	o.header_text = "";
	args_and_options.push_back({ cmd_line_9, o });
	reset_defaults(o);

	string cmd_line_10("C:/Libraries/stl-lite/array_test.exe -fn $cmd");
	o.fom = file_open_mode::new_file;
	o.output_filepath = "array_test.out";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_10, o });
	reset_defaults(o);

	string cmd_line_11("C:/Libraries/stl-lite/array_test.exe -fo $cmd");
	o.fom = file_open_mode::overwrite;
	o.output_filepath = "array_test.out";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_11, o });
	reset_defaults(o);

	string cmd_line_12("C:/Libraries/stl-lite/array_test.exe -fa $cmd");
	o.fom = file_open_mode::append;
	o.output_filepath = "array_test.out";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_12, o });
	reset_defaults(o);

	string cmd_line_13("C:/stl-lite/array_test.exe -run array_test;driver_test");
	o.suites_to_run = "array_test;driver_test";
	args_and_options.push_back({ cmd_line_13, o });
	reset_defaults(o);

	string cmd_line_14("C:/stl-lite/array_test.exe -h yes -ht \"Running $cmd\"");
	o.header = true;
	o.header_text = "Running array_test";
	args_and_options.push_back({ cmd_line_14, o });
	reset_defaults(o);

	string cmd_line_15("C:/stl-lite/array_test.exe -p auto -fn $cmd");
	o.prm = pass_report_mode::none;
	o.fom = file_open_mode::new_file;
	o.output_filepath = "array_test.out";
	args_and_options.push_back({ cmd_line_15, o });
	reset_defaults(o);

	string cmd_line_16("C:/stl-lite/array_test.exe -p auto");
	args_and_options.push_back({ cmd_line_16, o });

	string cmd_line_17("C:/stl-lite/array_test.exe -h no -s yes -p detail -t 3 -fn new_test");
	o.header = false;
	o.summary = true;
	o.prm = pass_report_mode::detail;
	o.fail_threshold = 3;
	o.fom = file_open_mode::new_file;
	o.output_filepath = "new_test";
	args_and_options.push_back({ cmd_line_17, o });
	reset_defaults(o);

	string cmd_line_18("C:/stl-lite/array_test.exe -h yes -ht \"Running $cmd\" -p none");
	o.header = true;
	o.header_text = "Running array_test";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_18, o });
	reset_defaults(o);

	string cmd_line_19("C:/stl-lite/array_test.exe -h yes -fn $cmd -p detail -fo $cmd -p auto");
	o.header = true;
	o.fom = file_open_mode::overwrite;
	o.prm = pass_report_mode::none;
	o.output_filepath = "array_test.out";
	args_and_options.push_back({ cmd_line_19, o });
	reset_defaults(o);

	string cmd_line_20("C:/stl-lite/array_test.exe -run array_test;string_view_test;driver_test");
	o.suites_to_run = "array_test;string_view_test;driver_test";
	args_and_options.push_back({ cmd_line_20, o });
	reset_defaults(o);

	string cmd_line_21("C:/stl-lite/array_test.exe -h no -s no -p detail -t 2 -fa a_file -run a_test_suite");
	o.header = false;
	o.summary = false;
	o.prm = pass_report_mode::detail;
	o.fail_threshold = 2;
	o.fom = file_open_mode::append;
	o.output_filepath = "a_file.out";
	o.suites_to_run = "a_test_suite";
	args_and_options.push_back({ cmd_line_21, o });

	return args_and_options;
}

void test_get_options()
{
	char* args[100];

	std::vector<std::pair<std::string, Options>> args_and_options = generate_expected_options();
	std::size_t cmd_line_number{ 0 };
	for (auto iter = args_and_options.begin(); iter != args_and_options.end(); ++iter)
	{
		std::vector<std::string> v = split(iter->first, ' ');
		for (std::size_t i = 0; i < v.size(); ++i)
			args[i] = v[i].data();

		Options o = get_options(args, v.size());

		compare(o, iter->second, "cmd_line_" + std::to_string(cmd_line_number));
		std::cout << o.header_text << '\n';
		++cmd_line_number;
	}
}