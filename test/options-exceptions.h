/*
* options-exceptions.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Define options-related exceptions
*/

#ifndef STL_LITE_OPTIONS_EXCEPTIONS_H
#define STL_LITE_OPTIONS_EXCEPTIONS_H

#include <stdexcept>
#include <string_view>
#include <string>
#include <filesystem>

static std::string message(const std::string_view& base, const std::string& extra = "")
{
	std::string msg{ base };
	if (!extra.empty())
		msg += ": " + extra;

	return msg;
}

//base class for cmd-line errors: no public ctors to force use of a specialized error
class cmd_line_error : public std::runtime_error {

protected:
	cmd_line_error(const std::string_view& base) : std::runtime_error(message(base)) {}

	cmd_line_error(const std::string_view& base, const std::string& details) : 
		std::runtime_error(message(base, details)),
		details_(details) {}

	const std::string& details() const noexcept
	{
		return details_;
	}

private:
	std::string details_;
};


//general error with cmd-line
class invalid_cmd_line : public cmd_line_error {

public:
	invalid_cmd_line() : cmd_line_error(base) {}
	invalid_cmd_line(const char* details) : invalid_cmd_line(std::string(details)) {}
	invalid_cmd_line(const std::string_view& details) : invalid_cmd_line(std::string(details)) {}
	invalid_cmd_line(const std::string& details) : cmd_line_error(base, details) {}

	const std::string& details() const noexcept
	{
		return cmd_line_error::details();
	}

private:
	static constexpr std::string_view base{ "invalid command line" };
};


//error with option name on the cmd-line
class invalid_option_name : public cmd_line_error
{

public:
	invalid_option_name() : cmd_line_error(base) {}
	invalid_option_name(const char* name) : invalid_option_name(std::string(name)) {}
	invalid_option_name(const std::string_view& name) : invalid_option_name(std::string(name)) {}
	invalid_option_name(const std::string& name) : cmd_line_error(base, name) {}

	const std::string& option_name() const noexcept
	{
		return cmd_line_error::details();
	}

private:
	static constexpr std::string_view base{ "invalid option name" };
};


//error with option value on the cmd-line
class invalid_option_value : public cmd_line_error
{

public:
	invalid_option_value() : cmd_line_error(base) {}
	invalid_option_value(const char* value) : invalid_option_value(std::string(value)) {}
	invalid_option_value(const std::string_view& value) : invalid_option_value(std::string(value)) {}
	invalid_option_value(const std::string& value) : cmd_line_error(base, value) {}

	const std::string& option_value() const noexcept
	{
		return cmd_line_error::details();
	}

private:
	static constexpr std::string_view base { "invalid option value" };
};


//error related to output file
class file_error : public std::runtime_error {

public:
	file_error(const std::string& base) : std::runtime_error(message(base)) {}

	file_error(const std::string& base, const std::filesystem::path& filepath) :
		std::runtime_error(message(base, filepath.string())),
		filepath_(filepath)
	{}

	const std::filesystem::path& filepath() const noexcept
	{
		return filepath_;
	}

private:
	std::filesystem::path filepath_;
};


#endif