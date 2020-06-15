/*
* verifiers.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Declare and define verifiers: verifiers are short-hand functions and call verify internally
* suite runners should call verifiers instead of calling verify function directly
*/

#ifndef STL_LITE_VERIFIERS_H
#define STL_LITE_VERIFIERS_H

#include <type_traits>

//intentionally declared here instead of including "tester.h"
//suite runners need access only to verifiers and nothing else in the tester
void verify(bool success, const char* msg);


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