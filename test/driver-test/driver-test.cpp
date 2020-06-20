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

#include "../verifiers.h"
#include "../options.h"
#include "../tester.h"
#include "../utils.h"

void test_get_options_nominal();
void verify_options(const Options& received, const Options& expected, const std::string& test_case);
void test_cmd_line(const std::string& cmd_line, const Options& expected);

//unit test for the test driver
void driver_test()
{
	test_get_options_nominal();
}


void test_get_options_default(const Options& template_options);
void test_get_options_single(const Options& template_options);
void test_get_options_combine(const Options& template_options);
void test_get_options_multiple_suites(const Options& template_options);
void test_get_options_specific(const Options& template_options);
void test_get_options_edge_case(const Options& template_options);

//unit test for nominal behavior of get_options
void test_get_options_nominal()
{
	Options template_options;
	template_options.command_name = "array_test";

	test_get_options_default(template_options);
	test_get_options_single(template_options);
	test_get_options_combine(template_options);
	test_get_options_multiple_suites(template_options);
	test_get_options_specific(template_options);
	test_get_options_edge_case(template_options);
}


//parse cmd-line to array similar to main's argv and verify if get_options returns expected options
void test_cmd_line(const std::string& cmd_line, const Options& expected)
{
	//set size to 100 to ensure enough room for many options in a single cmd-line
	char* args[100];

	std::vector<std::string> v = split(cmd_line, ' ');
	std::size_t size = v.size();
	for (std::size_t i = 0; i < size; ++i)
		args[i] = v[i].data();

	static unsigned short count{ 0 };
	const std::string test_case{ "cmd-line " + std::to_string(count++) };
	Options received = get_options(args, size);
	verify_options(received, expected, test_case);
}


void test_get_options_default(const Options& template_options)
{
	//default arguments
	std::string cmd_line("C:/Libraries/stl-lite/array_test.exe");
	test_cmd_line(cmd_line, template_options);
}


void test_get_options_single_h(const Options& template_options);
void test_get_options_single_p(const Options& template_options);
void test_get_options_single_s(const Options& template_options);
void test_get_options_single_t(const Options& template_options);
void test_get_options_single_fx(const Options& template_options);
void test_get_options_single_run(const Options& template_options);

void test_get_options_single(const Options& template_options)
{
	//single name-value option supplied
	test_get_options_single_h(template_options);
	test_get_options_single_p(template_options);
	test_get_options_single_s(template_options);
	test_get_options_single_t(template_options);
	test_get_options_single_fx(template_options);
	test_get_options_single_run(template_options);
}


void test_get_options_combine_h(const Options& template_options);
void test_get_options_combine_p(const Options& template_options);
void test_get_options_combine_random(const Options& template_options);

void test_get_options_combine(const Options& template_options)
{
	test_get_options_combine_h(template_options);
	test_get_options_combine_p(template_options);
	test_get_options_combine_random(template_options);
}


void test_get_options_multiple_suites(const Options& template_options)
{
	//multiple suites as values for -run
	std::string cmd_line = "C:/stl-lite/array_test.exe -run array_test;string_view_test;driver_test";
	Options expected_options = template_options;
	expected_options.suites_to_run = "array_test;string_view_test;driver_test";
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_specific(const Options& template_options)
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
	test_cmd_line(cmd_line, expected_options);

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
	test_cmd_line(cmd_line, expected_options);

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
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_edge_case(const Options& template_options)
{
	//edge case: fail threshold
	std::string cmd_line = "C:/stl-lite/array_test.exe -t max";
	Options expected_options = template_options;
	expected_options.fail_threshold = USHRT_MAX; //this may change due to changes made in milestone 5
	test_cmd_line(cmd_line, expected_options);

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
	test_cmd_line(cmd_line, expected_options);

	//edge case: header text
	cmd_line =
		"C:/array_test.exe -h yes -ht a_very_very_very_very_very_very_very_very_very_very_long_header_text";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "a_very_very_very_very_very_very_very_very_very_very_long_header_text";
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_single_h(const Options& template_options)
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -h no";
	Options expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -h yes";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "Running $suite";
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_single_p(const Options& template_options)
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -p none";
	Options expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -p indicate";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::indicate;
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -p detail";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::detail;
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_single_s(const Options& template_options)
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -s no";
	Options expected_options = template_options;
	expected_options.summary = false;
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -s yes";
	expected_options = template_options;
	expected_options.summary = true;
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_single_t(const Options& template_options)
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -t 5";
	Options expected_options = template_options;
	expected_options.fail_threshold = 5;
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_single_fx(const Options& template_options)
{
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -fn $cmd";
	Options expected_options = template_options;
	expected_options.fom = file_open_mode::new_file;
	expected_options.output_filepath = "array_test.out";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -fo $cmd";
	expected_options = template_options;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.output_filepath = "array_test.out";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/Libraries/stl-lite/array_test.exe -fa $cmd";
	expected_options = template_options;
	expected_options.fom = file_open_mode::append;
	expected_options.output_filepath = "array_test.out";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_single_run(const Options& template_options)
{
	std::string cmd_line = "C:/stl-lite/array_test.exe -run array_test";
	Options expected_options = template_options;
	expected_options.suites_to_run = "array_test";
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_combine_h(const Options& template_options)
{
	//no header but header text supplied
	std::string cmd_line = "C:/Libraries/stl-lite/array_test.exe -h no -ht should_be_ignored";
	Options expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	test_cmd_line(cmd_line, expected_options);

	//header yes and header text supplied
	cmd_line = "C:/stl-lite/array_test.exe -h yes -ht Running_$cmd";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "Running_array_test";
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_combine_p(const Options& template_options)
{
	//prm auto when fom != no_file
	std::string cmd_line = "C:/stl-lite/array_test.exe -p auto -fn $cmd";
	Options expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	expected_options.fom = file_open_mode::new_file;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/stl-lite/array_test.exe -p auto -fo $cmd";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/stl-lite/array_test.exe -p auto -fa $cmd";
	expected_options = template_options;
	expected_options.prm = pass_report_mode::none;
	expected_options.fom = file_open_mode::append;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options);

	//prm auto when fom == no_file
	cmd_line = "C:/stl-lite/array_test.exe -p auto";
	expected_options = template_options;
	expected_options = template_options;
	test_cmd_line(cmd_line, expected_options);
}


void test_get_options_combine_random(const Options& template_options)
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
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/stl-lite/array_test.exe -h yes -ht Running_$cmd_now -p none";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.header_text = "Running_array_test_now";
	expected_options.prm = pass_report_mode::none;
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/stl-lite/array_test.exe -h yes -fn $cmd -p detail -fo $cmd -p auto";
	expected_options = template_options;
	expected_options.header = true;
	expected_options.fom = file_open_mode::overwrite;
	expected_options.prm = pass_report_mode::none;
	expected_options.output_filepath = "array_test.out";
	test_cmd_line(cmd_line, expected_options);

	cmd_line = "C:/stl-lite/array_test.exe -h no -s no -p detail -t 2 -fa a_file -run a_test_suite";
	expected_options = template_options;
	expected_options.header = false;
	expected_options.header_text = "";
	expected_options.summary = false;
	expected_options.prm = pass_report_mode::detail;
	expected_options.fail_threshold = 2;
	expected_options.fom = file_open_mode::append;
	expected_options.output_filepath = "a_file.out";
	expected_options.suites_to_run = "a_test_suite";
	test_cmd_line(cmd_line, expected_options);
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