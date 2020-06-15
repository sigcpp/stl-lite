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
	char path[]{ "C:/Libraries/stl-lite/array-test.exe" }, h[]{ "-h" }, ht[]{ "-ht" }, p[]{ "-p" },
		s[]{ "-s" }, t[]{ "-t" }, run[]{ "-run" }, fn[]{ "-fn" }, fo[]{ "-fo" }, fa[]{ "-fa" };

	// values in name-value pair
	char yes[]{ "yes" }, no[]{ "no" }, thresh[]{ "3" }, cmd[]{ "$cmd" }, indicate[]{ "indicate" },
		detail[]{ "detail" }, none[]{ "none" }, array[]{ "array-test" };

	
	char* args[]{ path, h, yes, ht, cmd, p, detail, s, no, t, thresh, fn, cmd, run, array  };
	std::size_t size = (sizeof(args) / sizeof(args[0]));

	Options o = get_options(args, size);

	is_true(o.header == true, "-h yes");
	is_true(o.summary == false, "-s no");
	is_true(o.header_text == array, "-ht $cmd");
	is_true(o.prm == pass_report_mode::detail, "-p detail");
	is_true(o.fail_threshold == 3, "-t 3");
	is_true(o.command_name == array, "command_name");
	is_true(o.fom == file_open_mode::new_file && o.output_filepath == "array-test.out", "-fn $cmd");
	is_true(o.suites_to_run == array, "-run array-test");
}