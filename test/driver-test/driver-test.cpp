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

#include <iostream>
#include <vector>
#include <string>

#include "../verifiers.h"
#include "../options.h"
#include "../tester.h"
#include "../utils.h"

void driver_test();
void test_get_options();
void reset_default_options(Options& o);
void compare(const Options& received, const Options& expected, const std::string& test_case);
std::vector<std::pair<std::string, Options>> generate_cmdline_option_pairs();

//unit test for the test driver
void driver_test()
{
	test_get_options();
}


//unit test for get_options
void test_get_options()
{
	//this is meant to replicate the main() parameter char* argv[]
	//size 100 was chosen to ensure enough room for many options in a single command-line.
	char* args[100];

	const std::vector<std::pair<std::string, Options>> args_and_options = generate_cmdline_option_pairs();
	std::size_t cmd_line_number{ 0 };
	for (auto iter = args_and_options.begin(); iter != args_and_options.end(); ++iter)
	{
		std::vector<std::string> v = split(iter->first, ' ');
		for (std::size_t i = 0; i < v.size(); ++i)
			args[i] = v[i].data();

		Options o = get_options(args, v.size());

		compare(o, iter->second, "cmd_line_" + std::to_string(cmd_line_number));
		++cmd_line_number;
	}
}


//resets given Options& parameter data members to the default values
void reset_default_options(Options& o)
{
	o.header = true;
	o.summary = true;
	o.header_text = "Running $suite";
	o.prm = pass_report_mode::indicate;
	o.fail_threshold = 0;
	o.fom = file_open_mode::no_file;
	o.output_filepath = "";
	o.command_name = "array_test"; //default command_name is empty but this is expected value for all tests
	o.suites_to_run = "";
}


//compares received Options object data members to expected Options object data members
//see fn declaration of is_true for the behavior of the comparison
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


//generates a vector of pairs that holds a dummy command line and the expected Options object that would
//be created from get_options parsing the command line.
std::vector<std::pair<std::string, Options>> generate_cmdline_option_pairs()
{
	std::vector<std::pair<std::string, Options>> args_and_options;
	Options o;

	using std::string;

	//default arguments
	const string cmd_line_0("C:/Libraries/stl-lite/array_test.exe");
	o.command_name = "array_test";
	args_and_options.push_back({ cmd_line_0, o });
	reset_default_options(o);

	//single name-value option supplied
	const string cmd_line_1("C:/Libraries/stl-lite/array_test.exe -h no");
	o.header = false;
	o.header_text = "";
	args_and_options.push_back({ cmd_line_1, o });
	reset_default_options(o);

	const string cmd_line_2("C:/Libraries/stl-lite/array_test.exe -h yes");
	o.header = true;
	o.header_text = "Running $suite";
	args_and_options.push_back({ cmd_line_2, o });
	reset_default_options(o);

	const string cmd_line_3("C:/Libraries/stl-lite/array_test.exe -p none");
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_3, o });
	reset_default_options(o);

	const string cmd_line_4("C:/Libraries/stl-lite/array_test.exe -p indicate");
	o.prm = pass_report_mode::indicate;
	args_and_options.push_back({ cmd_line_4, o });
	reset_default_options(o);

	const string cmd_line_5("C:/Libraries/stl-lite/array_test.exe -p detail");
	o.prm = pass_report_mode::detail;
	args_and_options.push_back({ cmd_line_5, o });
	reset_default_options(o);

	const string cmd_line_6("C:/Libraries/stl-lite/array_test.exe -s no");
	o.summary = false;
	args_and_options.push_back({ cmd_line_6, o });
	reset_default_options(o);

	const string cmd_line_7("C:/Libraries/stl-lite/array_test.exe -s yes");
	o.summary = true;
	args_and_options.push_back({ cmd_line_7, o });
	reset_default_options(o);

	const string cmd_line_8("C:/Libraries/stl-lite/array_test.exe -t 5");
	o.fail_threshold = 5;
	args_and_options.push_back({ cmd_line_8, o });
	reset_default_options(o);

	const string cmd_line_9("C:/Libraries/stl-lite/array_test.exe -fn $cmd");
	o.fom = file_open_mode::new_file;
	o.output_filepath = "array_test.out";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_9, o });
	reset_default_options(o);

	const string cmd_line_10("C:/Libraries/stl-lite/array_test.exe -fo $cmd");
	o.fom = file_open_mode::overwrite;
	o.output_filepath = "array_test.out";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_10, o });
	reset_default_options(o);

	const string cmd_line_11("C:/Libraries/stl-lite/array_test.exe -fa $cmd");
	o.fom = file_open_mode::append;
	o.output_filepath = "array_test.out";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_11, o });
	reset_default_options(o);

	const string cmd_line_12("C:/stl-lite/array_test.exe -run array_test");
	o.suites_to_run = "array_test";
	args_and_options.push_back({ cmd_line_12, o });
	reset_default_options(o);

	//no header but header text supplied
	const string cmd_line_13("C:/Libraries/stl-lite/array_test.exe -h no -ht should_be_ignored");
	o.header = false;
	o.header_text = "";
	args_and_options.push_back({ cmd_line_13, o });
	reset_default_options(o);

	//header yes and header text supplied
	const string cmd_line_14("C:/stl-lite/array_test.exe -h yes -ht Running_$cmd");
	o.header = true;
	o.header_text = "Running_array_test";
	args_and_options.push_back({ cmd_line_14, o });
	reset_default_options(o);

	//prm auto when fom != no_file
	const string cmd_line_15("C:/stl-lite/array_test.exe -p auto -fn $cmd");
	o.prm = pass_report_mode::none;
	o.fom = file_open_mode::new_file;
	o.output_filepath = "array_test.out";
	args_and_options.push_back({ cmd_line_15, o });
	reset_default_options(o);

	const string cmd_line_16("C:/stl-lite/array_test.exe -p auto -fo $cmd");
	o.prm = pass_report_mode::none;
	o.fom = file_open_mode::overwrite;
	o.output_filepath = "array_test.out";
	args_and_options.push_back({ cmd_line_16, o });
	reset_default_options(o);

	const string cmd_line_17("C:/stl-lite/array_test.exe -p auto -fa $cmd");
	o.prm = pass_report_mode::none;
	o.fom = file_open_mode::append;
	o.output_filepath = "array_test.out";
	args_and_options.push_back({ cmd_line_17, o });
	reset_default_options(o);

	//prm auto when fom == no_file
	const string cmd_line_18("C:/stl-lite/array_test.exe -p auto");
	args_and_options.push_back({ cmd_line_18, o });

	//random combinations of options
	const string cmd_line_19("C:/stl-lite/array_test.exe -h no -s yes -p detail -t 3 -fn new_test");
	o.header = false;
	o.header_text = "";
	o.summary = true;
	o.prm = pass_report_mode::detail;
	o.fail_threshold = 3;
	o.fom = file_open_mode::new_file;
	o.output_filepath = "new_test.out";
	args_and_options.push_back({ cmd_line_19, o });
	reset_default_options(o);

	const string cmd_line_20("C:/stl-lite/array_test.exe -h yes -ht Running_$cmd_now -p none");
	o.header = true;
	o.header_text = "Running_array_test_now";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_20, o });
	reset_default_options(o);

	const string cmd_line_21("C:/stl-lite/array_test.exe -h yes -fn $cmd -p detail -fo $cmd -p auto");
	o.header = true;
	o.fom = file_open_mode::overwrite;
	o.prm = pass_report_mode::none;
	o.output_filepath = "array_test.out";
	args_and_options.push_back({ cmd_line_21, o });
	reset_default_options(o);

	const string cmd_line_22(
		"C:/stl-lite/array_test.exe -h no -s no -p detail -t 2 -fa a_file -run a_test_suite");
	o.header = false;
	o.header_text = "";
	o.summary = false;
	o.prm = pass_report_mode::detail;
	o.fail_threshold = 2;
	o.fom = file_open_mode::append;
	o.output_filepath = "a_file.out";
	o.suites_to_run = "a_test_suite";
	args_and_options.push_back({ cmd_line_22, o });
	reset_default_options(o);

	//multiple suites as values for -run
	const string cmd_line_23("C:/stl-lite/array_test.exe -run array_test;string_view_test;driver_test");
	o.suites_to_run = "array_test;string_view_test;driver_test";
	args_and_options.push_back({ cmd_line_23, o });
	reset_default_options(o);

	//every option specified
	const string cmd_line_24(
		"C:/stl-lite/array_test.exe -h yes -ht Running_$cmd -s yes -p detail -t 2 -fa a_file -run tester");
	o.header = true;
	o.header_text = "Running_array_test";
	o.summary = true;
	o.prm = pass_report_mode::detail;
	o.fail_threshold = 2;
	o.fom = file_open_mode::append;
	o.output_filepath = "a_file.out";
	o.suites_to_run = "tester";
	args_and_options.push_back({ cmd_line_24, o });
	reset_default_options(o);

	const string cmd_line_25(
		"C:/stl-lite/array_test.exe -h yes -ht Running_$cmd -s no -p indicate -t 10 -fo a_file -run tester");
	o.header = true;
	o.header_text = "Running_array_test";
	o.summary = false;
	o.prm = pass_report_mode::indicate;
	o.fail_threshold = 10;
	o.fom = file_open_mode::overwrite;
	o.output_filepath = "a_file.out";
	o.suites_to_run = "tester";
	args_and_options.push_back({ cmd_line_25, o });
	reset_default_options(o);

	const string cmd_line_26(
		"C:/stl-lite/array_test.exe -h no -ht $cmd -s yes -p none -t 2 -fn a_file -run tester");
	o.header = false;
	o.header_text = "";
	o.summary = true;
	o.prm = pass_report_mode::none;
	o.fail_threshold = 2;
	o.fom = file_open_mode::new_file;
	o.output_filepath = "a_file.out";
	o.suites_to_run = "tester";
	args_and_options.push_back({ cmd_line_26, o });
	reset_default_options(o);

	//edge case: fail threshold
	const string cmd_line_27("C:/stl-lite/array_test.exe -t 65535"); //USHRT_MAX: compiler specific
	o.fail_threshold = USHRT_MAX; //this may change due to changes made in milestone 5
	args_and_options.push_back({ cmd_line_27, o });
	reset_default_options(o);

	//edge case: output filepath
	//max filepath length in Windows, Linux and OS X is 255 characters
	//the length of this filename is 75 characters
	const string cmd_line_28(
		"C:/array_test.exe -fo a_very_very_very_very_very_very_very_very_very_very_very_very_long_filename");
	o.fom = file_open_mode::overwrite;
	o.output_filepath = "a_very_very_very_very_very_very_very_very_very_very_very_very_long_filename.out";
	o.prm = pass_report_mode::none;
	args_and_options.push_back({ cmd_line_28, o });
	reset_default_options(o);

	//edge case: header text
	const string cmd_line_29(
		"C:/array_test.exe -h yes -ht a_very_very_very_very_very_very_very_very_very_very_long_header_text");
	o.header = true;
	o.header_text = "a_very_very_very_very_very_very_very_very_very_very_long_header_text";
	args_and_options.push_back({ cmd_line_29, o });

	return args_and_options;
}