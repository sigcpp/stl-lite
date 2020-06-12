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

enum class pass_report_mode { none, indicate, detail };

void set_header_text(std::string text);
void set_pass_report_mode(pass_report_mode mode);
void set_fail_threshold(unsigned short value);
void set_max_fail_threshold();
void set_output(std::ostream& o);

unsigned get_tests_done();
unsigned get_tests_failed();

void verify(bool success, const char* msg);
void summarize_tests();

void log(const char* s);
void logLine(const char* s);

#endif