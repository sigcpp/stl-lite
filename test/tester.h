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

#ifndef STL_LITE_TESTER_H
#define STL_LITE_TESTER_H

#include <iostream>
#include <type_traits>


enum class pass_report_mode { none, indicate, detail };

void set_header_text(std::string text);
void set_pass_report_mode(pass_report_mode mode);
void set_fail_threshold(int value);
void set_max_fail_threshold();
void set_output(std::ostream& o);

void log(const char* s);
void log_line(const char* s);

int get_tests_failed_total();

void start_suite(const std::string& name);

void summarize_suite();
void summarize_tests();

#endif
