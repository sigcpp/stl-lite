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

#include <iostream>
#include <string_view>

void driver_test()
{
	//names in name-value pair
	char path[]{ "C:/Libraries/stl-lite/array_test.exe" }, h[]{ "-h" }, ht[]{ "-ht" }, p[]{ "-p" },
		s[]{ "-s" }, t[]{ "-t" }, run[]{ "-run" }, fn[]{ "-fn" }, fo[]{ "-fo" }, fa[]{ "-fa" };

	// values in name-value pair
	char yes[]{ "yes" }, no[]{ "no" }, thresh[]{ "3" }, cmd[]{ "$cmd" }, indicate[]{ "indicate" },
		detail[]{ "detail" }, none[]{ "none" }, command_name[]{ "array_test" };

	//array with only path to test the default behavior
	char* default_args[]{ path };
	std::size_t size = (sizeof(default_args) / sizeof(default_args[0]));

	Options o = get_options(default_args, size);

	is_true(o.header == true, "Default header = yes");
	is_true(o.summary == true, "Default summary = yes");
	is_true(o.header_text == "Running $suite", "Default header text");
	is_true(o.prm == pass_report_mode::indicate, "Default PRM w/ no file output = indicate");
	is_true(o.fail_threshold == 0, "Default fail threshold = 0");
	is_true(o.command_name == command_name, "command_name");
	is_true(o.fom == file_open_mode::no_file, "Default FOM = no_file");

	//array with dummy arguments to test behavior of explicit options
	char* args[]{ path, h, no, p, detail, s, no, t, thresh, fn, cmd, run, command_name  };
	size = (sizeof(args) / sizeof(args[0]));

	o = get_options(args, size);

	is_true(o.header == false, "-h no");
	is_true(o.summary == false, "-s no");
	is_true(o.prm == pass_report_mode::detail, "-p detail");
	is_true(o.fail_threshold == 3, "-t 3");
	is_true(o.command_name == command_name, "command_name");
	is_true(o.fom == file_open_mode::new_file && o.output_filepath == "array_test.out", "-fn $cmd");
	is_true(o.suites_to_run == command_name, "-run array_test");

	args[9] = fo;
	args[4] = none;

	o = get_options(args, size);
	is_true(o.fom == file_open_mode::overwrite && o.output_filepath == "array_test.out", "-fo $cmd");
	is_true(o.prm == pass_report_mode::none, "-p none");

	args[9] = fa;
	args[4] = indicate;
	o = get_options(args, size);
	is_true(o.fom == file_open_mode::append && o.output_filepath == "array_test.out", "-fa $cmd");
	is_true(o.prm == pass_report_mode::indicate, "-p indicate");

	args[2] = yes;
	args[3] = ht;
	args[4] = cmd;
	o = get_options(args, size);
	is_true(o.header == true, "-h yes");
	is_true(o.header_text == command_name, "-ht $cmd");
}