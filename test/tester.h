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
void set_fail_threshold(unsigned short value);
void set_max_fail_threshold();
void set_output(std::ostream& o);

void log(const char* s);
void log_line(const char* s);

unsigned get_tests_failed_total();

void start_suite(const std::string& name);

void summarize_suite();
void summarize_tests();

void verify(bool success, const char* msg);

// the following functions and templates are helper functions short-hand to verify
// call the helpers instead of calling verify directly

inline void is_true(bool value, const char* msg)
{
	verify(value, msg);
}


inline void is_false(bool value, const char* msg)
{
	verify(!value, msg);
}


//TODO replace this template with a macro to reduce template instantiations
template <typename T>
constexpr bool is_nonbool_arithmetic()
{
	return std::is_integral_v<T> && !std::is_same_v<T, bool>;
}


template <typename T = int>
inline void is_zero(T value, const char* msg)
{
	static_assert(is_nonbool_arithmetic<T>(), "requires non-bool arithmetic type");
	verify(value == 0, msg);
}


template <typename T = int>
inline void is_nonzero(T value, const char* msg)
{
	static_assert(is_nonbool_arithmetic<T>(), "requires non-bool arithmetic type");
	verify(value != 0, msg);
}


template <typename T = int>
inline void is_negative(T value, const char* msg)
{
	static_assert(is_nonbool_arithmetic<T>(), "requires non-bool arithmetic type");
	verify(value < 0, msg);
}


template <typename T = int>
inline void is_nonnegative(T value, const char* msg)
{
	static_assert(is_nonbool_arithmetic<T>(), "requires non-bool arithmetic type");
	verify(value >= 0, msg);
}


template <typename T = int>
inline void is_positive(T value, const char* msg)
{
	static_assert(is_nonbool_arithmetic<T>(), "requires non-bool arithmetic type");
	verify(value > 0, msg);
}

#endif