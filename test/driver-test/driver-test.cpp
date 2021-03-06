/*
* driver-test.cpp
* Nick DeMarco, Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Unit tests for test driver
*/

#include <vector>
#include <string>
#include <climits>

#include "../verifiers.h"
#include "../options.h"
#include "../tester.h"
#include "../utils.h"

void test_get_options_nominal();
void verify_options(const Options& received, const Options& expected, const std::string& test_case);
void test_cmd_line(const std::string& cmd_line, const Options& expected, const std::string& test_case);


//unit tests for the test driver
void driver_test()
{
	test_get_options_nominal();
}


//helper function to create options with specific values for selected members
//can avoid this function with designated initializers in C++20
static Options get_template_options()
{
	Options o;
	o.command_name = "array_test";
	return o;
}


//template options used to setup expected options in all nominal tests of get_options
//use designated initializers in C++20 instead of calling get_template_options
static const Options template_options = get_template_options();


void test_get_options_default();
void test_get_options_single();
void test_get_options_combine();
void test_get_options_multiple_suites();
void test_get_options_specific();
void test_get_options_edge_case();


//unit test for nominal behavior of get_options
void test_get_options_nominal()
{
	test_get_options_default();
	test_get_options_single();
	test_get_options_combine();
	test_get_options_multiple_suites();
	test_get_options_specific();
	test_get_options_edge_case();
}


//parse cmd-line to array similar to main's argv and verify if get_options returns expected options
void test_cmd_line(const std::string& cmd_line, const Options& expected, const std::string& test_case)
{
	//set size to 100 to ensure enough room for many options in a single cmd-line
	char* args[100];

	std::vector<std::string> v = split(cmd_line, ' ');
	std::size_t size = v.size();
	for (std::size_t i = 0; i < size; ++i)
		args[i] = v[i].data();

	Options received = get_options(args, size);
	verify_options(received, expected, test_case);
}


void test_get_options_default()
{
	//default arguments
	std::string cmd_line("C:/Libraries/stl-lite/array_test.exe");
	test_cmd_line(cmd_line, template_options, "cmd-line 1");
}


void test_get_options_single_h();
void test_get_options_single_p();
void test_get_options_single_s();
void test_get_options_single_t();
void test_get_options_single_fx();
void test_get_options_single_run();
void test_get_options_command_name();

void test_get_options_single()
{
	//single name-value option supplied
	test_get_options_single_h();
	test_get_options_single_p();
	test_get_options_single_s();
	test_get_options_single_t();
	test_get_options_single_fx();
	test_get_options_single_run();
	test_get_options_command_name();
}


void test_get_options_combine_h();
void test_get_options_combine_p();
void test_get_options_combine_arbitrary();

void test_get_options_combine()
{
	test_get_options_combine_h();
	test_get_options_combine_p();
	test_get_options_combine_arbitrary();
}


void test_get_options_single_h()
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -h no";
	Options expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	test_cmd_line(cmd_line, expected_options, "cmd-line 2");

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -h yes";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "Running $suite";
	test_cmd_line(cmd_line, expected_options, "cmd-line 3");
}


void test_get_options_single_p()
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -p none";
	Options expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options, "cmd-line 4");

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -p indicate";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::indicate;
	test_cmd_line(cmd_line, expected_options, "cmd-line 5");

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -p detail";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::detail;
	test_cmd_line(cmd_line, expected_options, "cmd-line 6");
}


void test_get_options_single_s()
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -s no";
	Options expected_options = template_options;
	expected_options.summary = false;
	test_cmd_line(cmd_line, expected_options, "cmd-line 7");

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -s yes";
	expected_options = template_options;
	expected_options.summary = true;
	test_cmd_line(cmd_line, expected_options, "cmd-line 8");
}


void test_get_options_single_t()
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -t 5";
	Options expected_options = template_options;
	expected_options.fail_threshold = 5;
	test_cmd_line(cmd_line, expected_options, "cmd-line 9");
}


void test_get_options_single_fx()
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -fn $cmd";
	Options expected_options = template_options;
	expected_options.fom = file_open_mode::new_file;
	expected_options.output_filepath = "array_test.out";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options, "cmd-line 10");

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -fo $cmd";
	expected_options = template_options;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.output_filepath = "array_test.out";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options, "cmd-line 11");

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -fa $cmd";
	expected_options = template_options;
	expected_options.fom = file_open_mode::append;
	expected_options.output_filepath = "array_test.out";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options, "cmd-line 12");
}


void test_get_options_single_run()
{
	std::string cmd_line = "C:/stl-lite/array_test.exe -run array_test";
	Options expected_options = template_options;
	expected_options.suites_to_run = "array_test";
	test_cmd_line(cmd_line, expected_options, "cmd-line 13");
}


void test_get_options_command_name()
{
	//different filename
	std::string cmd_line = "C:/test_suites/some_more_test_suites/even_more_test_suites/a_test_suite.exe";
	Options expected_options = template_options;
	expected_options.command_name = "a_test_suite";
	test_cmd_line(cmd_line, expected_options, "cmd-line 14");

	//different drive letter
	cmd_line = "D:/a_folder/another_folder/yet_another_folder/edwall/test.exe";
	expected_options = template_options;
	expected_options.command_name = "test";
	test_cmd_line(cmd_line, expected_options, "cmd-line 15");

	//in root directory
	cmd_line = "E:/sample_test.exe";
	expected_options = template_options;
	expected_options.command_name = "sample_test";
	test_cmd_line(cmd_line, expected_options, "cmd-line 16");

	//no drive letter or path
	cmd_line = "driver.exe";
	expected_options = template_options;
	expected_options.command_name = "driver";
	test_cmd_line(cmd_line, expected_options, "cmd-line 17");

	//no file extension
	cmd_line = "F:/test_suites/multi-drive";
	expected_options = template_options;
	expected_options.command_name = "multi-drive";
	test_cmd_line(cmd_line, expected_options, "cmd-line 18");

	//relative path and non-exe extension
	cmd_line = "bin/driver.com";
	expected_options = template_options;
	expected_options.command_name = "driver";
	test_cmd_line(cmd_line, expected_options, "cmd-line 19");
}


void test_get_options_combine_h()
{
	//no header but header text supplied
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -h no -ht should_be_ignored";
	Options expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	test_cmd_line(cmd_line, expected_options, "cmd-line 20");

	//header yes and header text supplied
	cmd_line = "C:/stl-lite/array_test.exe -h yes -ht Running_$cmd";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "Running_array_test";
	test_cmd_line(cmd_line, expected_options, "cmd-line 21");
}


void test_get_options_combine_p()
{
	//prm auto when fom != no_file
	std::string cmd_line = "C:/stl-lite/array_test.exe -p auto -fn $cmd";
	Options expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	expected_options.fom = file_open_mode::new_file;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options, "cmd-line 22");

	cmd_line = "C:/stl-lite/array_test.exe -p auto -fo $cmd";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options, "cmd-line 23");

	cmd_line = "C:/stl-lite/array_test.exe -p auto -fa $cmd";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	expected_options.fom = file_open_mode::append;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options, "cmd-line 24");

	//prm auto when fom == no_file
	cmd_line = "C:/stl-lite/array_test.exe -p auto";
	expected_options = template_options;
	expected_options = template_options;
	test_cmd_line(cmd_line, expected_options, "cmd-line 25");
}


void test_get_options_combine_arbitrary()
{
	//random combinations of options
	std::string cmd_line = "C:/stl-lite/array_test.exe -h no -s yes -p detail -t 3 -fn new_test";
	Options expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	expected_options.summary = true;
	expected_options.prm = pass_report_mode::detail;
	expected_options.fail_threshold = 3;
	expected_options.fom = file_open_mode::new_file;
	expected_options.output_filepath = "new_test.out";
	test_cmd_line(cmd_line, expected_options, "cmd-line 26");

	cmd_line = "C:/stl-lite/map_test.exe -h yes -ht Running_$cmd_now -p none";
	expected_options = template_options;
	expected_options.command_name = "map_test";
	expected_options.header = true;
	expected_options.header_text = "Running_map_test_now";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options, "cmd-line 27");

	cmd_line = "C:/stl-lite/array_test.exe -h yes -fn $cmd -p detail -fo $cmd -p auto";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.prm = pass_report_mode::none;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options, "cmd-line 28");

	cmd_line = "C:/stl-lite/stl-lite_test.exe -h no -s no -p detail -t 2 -fa a_file -run a_test_suite";
	expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	expected_options.summary = false;
	expected_options.prm = pass_report_mode::detail;
	expected_options.fail_threshold = 2;
	expected_options.fom = file_open_mode::append;
	expected_options.output_filepath = "a_file.out";
	expected_options.suites_to_run = "a_test_suite";
	expected_options.command_name = "stl-lite_test";
	test_cmd_line(cmd_line, expected_options, "cmd-line 29");
}


void test_get_options_multiple_suites()
{
	//multiple suites as values for -run
	std::string cmd_line = "C:/stl-lite/array_test.exe -run array_test;string_view_test;driver_test";
	Options expected_options = template_options;
	expected_options.suites_to_run = "array_test;string_view_test;driver_test";
	test_cmd_line(cmd_line, expected_options, "cmd-line 30");
}


void test_get_options_specific()
{
	//every option specified
	std::string cmd_line =
		"C:/stl-lite/array_test.exe -h yes -ht Running_$cmd -s yes -p detail -t 2 -fa a_file -run tester";
	Options expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "Running_array_test";
	expected_options.summary = true;
	expected_options.prm = pass_report_mode::detail;
	expected_options.fail_threshold = 2;
	expected_options.fom = file_open_mode::append;
	expected_options.output_filepath = "a_file.out";
	expected_options.suites_to_run = "tester";
	test_cmd_line(cmd_line, expected_options, "cmd-line 31");

	cmd_line =
		"C:/stl-lite/array_test.exe -h yes -ht Running_$cmd -s no -p indicate -t 10 -fo a_file -run tester";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "Running_array_test";
	expected_options.summary = false;
	expected_options.prm = pass_report_mode::indicate;
	expected_options.fail_threshold = 10;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.output_filepath = "a_file.out";
	expected_options.suites_to_run = "tester";
	test_cmd_line(cmd_line, expected_options, "cmd-line 32");

	cmd_line =
		"C:/stl-lite/array_test.exe -h no -ht $cmd -s yes -p none -t 2 -fn a_file -run tester";
	expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	expected_options.summary = true;
	expected_options.prm = pass_report_mode::none;
	expected_options.fail_threshold = 2;
	expected_options.fom = file_open_mode::new_file;
	expected_options.output_filepath = "a_file.out";
	expected_options.suites_to_run = "tester";
	test_cmd_line(cmd_line, expected_options, "cmd-line 33");
}


void test_get_options_edge_case()
{
	//edge case: fail threshold
	std::string cmd_line = "C:/stl-lite/array_test.exe -t max";
	Options expected_options = template_options;
	expected_options.fail_threshold = INT_MAX;
	test_cmd_line(cmd_line, expected_options, "cmd-line 34");

	//edge case: output filepath
	//max filepath length in Windows, Linux and OS X is 255 characters
	//the length of this filename is 75 characters
	cmd_line =
		"C:/array_test.exe -fo a_very_very_very_very_very_very_very_very_very_very_very_very_long_filename";
	expected_options = template_options;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.output_filepath =
		"a_very_very_very_very_very_very_very_very_very_very_very_very_long_filename.out";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options, "cmd-line 35");

	//edge case: header text
	cmd_line =
		"C:/array_test.exe -h yes -ht a_very_very_very_very_very_very_very_very_very_very_long_header_text";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "a_very_very_very_very_very_very_very_very_very_very_long_header_text";
	test_cmd_line(cmd_line, expected_options, "cmd-line 36");

	//edge case: command_name
	cmd_line =
		"C:/a_very_very_very_very_very_very_very_very_very_very_very_very_very_very_very_long_command_name";
	expected_options = template_options;
	expected_options.command_name =
		"a_very_very_very_very_very_very_very_very_very_very_very_very_very_very_very_long_command_name";
	test_cmd_line(cmd_line, expected_options, "cmd-line 37");
}


//perform pair-wise verification of received options and expected options
void verify_options(const Options& received, const Options& expected, const std::string& test_case)
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