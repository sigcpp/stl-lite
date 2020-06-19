/*
* verifier-exceptions.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Define verifier-related exceptions
*/

#ifndef STL_LITE_VERIFIER_EXCEPTIONS_H
#define STL_LITE_VERIFIER_EXCEPTIONS_H

#include <stdexcept>
#include <string_view>
#include <string>

#include "utils.h"

//base class for verifier errors
class verifier_error : public std::runtime_error {

public:
	verifier_error(const std::string_view& base) : std::runtime_error{ format_message(base) } {}

	verifier_error(const std::string_view& base, const std::string& details) :
		std::runtime_error{ format_message(base, details) },
		details_{ details } {}

	const std::string& details() const noexcept
	{
		return details_;
	}

private:
	std::string details_;
};


class fail_threshold_met_error : public verifier_error {

public:
	fail_threshold_met_error() : verifier_error{ base } {}

private:
	static constexpr std::string_view base{ "fail threshold met" };
};

#endif
