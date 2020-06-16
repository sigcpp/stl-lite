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
#include <limits>


//type and limit of fail threshold
using fail_threshold_type = unsigned short;
static constexpr fail_threshold_type max_fail_threshold{ std::numeric_limits<fail_threshold_type>::max() };

enum class pass_report_mode { none, indicate, detail };

void set_header_text(std::string text);
void set_pass_report_mode(pass_report_mode mode);
void set_fail_threshold(fail_threshold_type value);
void set_max_fail_threshold();
void set_output(std::ostream& o);

void log(const char* s);
void log_line(const char* s);

unsigned get_tests_failed_total();

void start_suite(const std::string& name);

void summarize_suite();
void summarize_tests();

#endif
