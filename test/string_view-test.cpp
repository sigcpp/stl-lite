/*
* string_view-test.cpp
* Sean Murthy, Shengrui Chen
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Test string_view template
* - see C++17 [string.view]
*	https://timsong-cpp.github.io/cppwp/n4659/string.view
*/

#include <assert.h>
#include <stdexcept>

#include "tester.h"

void runTests()
{
	using std::string_view;

	//1. Construction and assignment
	//constructs an empty string_view
	string_view empty_sv;

	const char* str = "hello, world!";
	//constructs a string_view with const char
	string_view sv(str);
	//constructs a fixed-length string_view with const char
	string_view f_sv(str, 5);
	//constructs a string_view with reference
	string_view r_sv(sv);
	//constructs a string_view with assignment
	string_view a_sv = sv;

	//2. Capacity
	verify(empty_sv.size() == 0, "empty_sv.size()");
	verify(sv.size() == 13, "sv.size()");
	verify(f_sv.size() == 5, "f_sv.size()");
	verify(r_sv.size() == sv.size(), "r_sv.size()");
	verify(a_sv.size() == sv.size(), "a_sv.size()");

	verify(empty_sv.length() == empty_sv.size(), "empty_sv.length()");
	verify(sv.length() == sv.size(), "sv.length()");
	verify(f_sv.length() == f_sv.size(), "f_sv.length()");
	verify(r_sv.length() == r_sv.size(), "r_sv.length()");
	verify(a_sv.length() == a_sv.size(), "a_sv.length()");

	verify(empty_sv.empty(), "empty_sv.empty()");
	verify(!sv.empty(), "!sv.empty()");
	verify(!f_sv.empty(), "!f_sv.empty()");
	verify(!r_sv.empty(), "!r_sv.empty()");
	verify(!a_sv.empty(), "!a_sv.empty()");

	//3. Non-member comparison functions
	string_view s("A");
	string_view t("a");
	string_view p("A");
	verify(!(s == t), "s==t");
	verify(s == p, "s==p");
	verify(p == s, "p==s");
	verify(!(s != p), "s != p");
	verify(s != t, "s != t");

	verify(!(s > t), "s > t");
	verify(s < t, "s > t");
	verify(t > s, "t > s");
	verify(!(t < s), "t > s");
	verify(!(s > p), "s > t");
	verify(!(s < p), "s > t");

	verify(!(s >= t), "s > t");
	verify(s <= t, "s > t");
	verify(t >= s, "t > s");
	verify(!(t <= s), "t > s");
	verify(s >= p, "s > t");
	verify(s <= p, "s > t");

	//4. Element data and access
	verify(empty_sv.data() == nullptr, "empty_sv.data()");
	verify((sv.data() != nullptr && sv.data() == str), "sv.data()");
	verify((f_sv.data() != nullptr && f_sv.data() == str), "f_sv.data()");
	verify((r_sv.data() != nullptr && r_sv.data() == str), "r_sv.data()");
	verify((a_sv.data() != nullptr && a_sv.data() == str), "a_sv.data()");

	//normal
	verify(sv[0] == str[0], "sv[0]");
	verify(sv[3] == str[3], "sv[3]");
	verify(sv[6] == str[6], "sv[6]");
	verify(sv[9] == str[9], "sv[9]");
	verify(sv[12] == str[12], "sv[12]");

	verify(sv.at(0) == str[0], "sv.at(0)");
	verify(sv.at(3) == str[3], "sv.at(3)");
	verify(sv.at(6) == str[6], "sv.at(6)");
	verify(sv.at(9) == str[9], "sv.at(9)");
	verify(sv.at(12) == str[12], "sv.at(12)");

	verify(sv.front() == str[0], "sv.front()");
	verify(sv.back() == str[sv.size() - 1], "sv.back()");

	//fixed-length
	verify(f_sv[0] == str[0], "f_sv[0]");
	verify(f_sv[2] == str[2], "f_sv[2]");
	verify(f_sv[4] == str[4], "f_sv[4]");

	verify(f_sv.at(0) == str[0], "f_sv.at(0)");
	verify(f_sv.at(2) == str[2], "f_sv.at(2)");
	verify(f_sv.at(4) == str[4], "f_sv.at(4)");

	verify(f_sv.front() == str[0], "f_sv.front()");
	verify(f_sv.back() == str[f_sv.size() - 1], "f_sv.back()");

	//reference
	verify(r_sv[0] == str[0], "r_sv[0]");
	verify(r_sv[3] == str[3], "r_sv[3]");
	verify(r_sv[6] == str[6], "r_sv[6]");
	verify(r_sv[9] == str[9], "r_sv[9]");
	verify(r_sv[12] == str[12], "r_sv[12]");

	verify(r_sv.at(0) == str[0], "r_sv.at(0)");
	verify(r_sv.at(3) == str[3], "r_sv.at(3)");
	verify(r_sv.at(6) == str[6], "r_sv.at(6)");
	verify(r_sv.at(9) == str[9], "r_sv.at(9)");
	verify(r_sv.at(12) == str[12], "r_sv.at(12)");

	verify(r_sv.front() == str[0], "r_sv.front()");
	verify(r_sv.back() == str[r_sv.size() - 1], "r_sv.back()");

	//assignment
	verify(a_sv[0] == str[0], "a_sv[0]");
	verify(a_sv[3] == str[3], "a_sv[3]");
	verify(a_sv[6] == str[6], "a_sv[6]");
	verify(a_sv[9] == str[9], "a_sv[9]");
	verify(a_sv[12] == str[12], "a_sv[12]");

	verify(a_sv.at(0) == str[0], "a_sv.at(0)");
	verify(a_sv.at(3) == str[3], "a_sv.at(3)");
	verify(a_sv.at(6) == str[6], "a_sv.at(6)");
	verify(a_sv.at(9) == str[9], "a_sv.at(9)");
	verify(a_sv.at(12) == str[12], "a_sv.at(12)");

	verify(a_sv.front() == str[0], "a_sv.front()");
	verify(a_sv.back() == str[a_sv.size() - 1], "a_sv.back()");

	//5. Iterator support
	//forward iterators
	string_view svOriginal("some read-only text.");
	string_view svExpected("some read-only text.");

	bool iteratorTest = true;
	std::size_t i = 0;
	for (auto it = svOriginal.begin()
		; it != svOriginal.end() && iteratorTest; ++it, ++i)
		iteratorTest = *it == svExpected[i];
	verify(iteratorTest, "forward iterator");

	iteratorTest = true;
	i = 0;
	for (auto it = svOriginal.cbegin()
		; it != svOriginal.cend() && iteratorTest; ++it, ++i)
		iteratorTest = *it == svExpected[i];
	verify(iteratorTest, "const forward iterator");

	//reverse iterators
	string_view svrExpected(".txet ylno-daer emos");

	iteratorTest = true;
	i = 0;
	for (auto it = svOriginal.rbegin()
		; it != svOriginal.rend() && iteratorTest; ++it, ++i)
		iteratorTest = *it == svrExpected[i];
	verify(iteratorTest, "reverse iterator");

	iteratorTest = true;
	i = 0;
	for (auto it = svOriginal.crbegin()
		; it != svOriginal.crend() && iteratorTest; ++it, ++i)
		iteratorTest = *it == svrExpected[i];
	verify(iteratorTest, "const reverse iterator");

	//iterator on empty string_view: the loop body should not execute
	iteratorTest = true;
	for (const auto emp : empty_sv)
		iteratorTest = false;
	verify(iteratorTest, "fwd iterator on empty array");

	//6. Modifiers
	const char* sv_data{ sv.data() };
	size_t sv_size{ sv.size() };
	size_t n{ 1 };

	assert(n <= sv_size);
	sv.remove_prefix(n);
	sv_size -= n;
	verify((sv.data() == sv_data + 1 && sv.size() == sv_size)
		, "sv.remove_prefix()");
	sv_data = sv.data();

	assert(n <= sv_size);
	sv.remove_suffix(n);
	sv_size -= n;
	verify((sv.data() == sv_data && sv.size() == sv_size)
		, "sv.remove_suffix()");

	//swap(exchanges the values of *this and s.)
	string_view o("thisis");
	s = "string";
	string_view oExpected("string");
	string_view sExpected("thisis");
	o.swap(s);
	bool swapTest = true;
	for (std::size_t idx = 0; idx < o.size() && swapTest; ++idx)
		swapTest = o[idx] == oExpected[idx] && s[idx] == sExpected[idx];
	verify(swapTest, "o.swap(s)");

	//7. String operations
	//copy
	char str_copy[]{ "____o" };
	size_t rlen, pos{ 0 };
	n = 4;

	rlen = f_sv.copy(str_copy, n, pos);
	verify(rlen == (n < f_sv.size() - pos) ? n : f_sv.size() - pos
		, "f_sv.copy(str_copy, n, pos)");
	try
	{
		f_sv.copy(str_copy, n, f_sv.size() + 1);
		verify(false, "f_sv.copy(str_copy, n, pos), should not be executed.");
	}
	catch (const std::out_of_range&)
	{
		verify(true, "if pos > size(), then throw out_of_range exception");
	}
	catch (...)
	{
		verify(false, "Throw an unexpected exception.");
	}

	//substr
	sv = "something";
	pos = 4;
	n = 5;
	string_view sub_sv = sv.substr(pos, n);

	verify((sub_sv.data() == sv.data() + pos
		&& sub_sv.size() == (n < sv.size() - pos) ? n : sv.size() - pos)
		, "sv.copy(pos, n)");
	try
	{
		pos = sv.size() + 1;
		sub_sv = sv.substr(pos, n);
		verify(false, "sv.substr(pos, n), should not be executed.");
	}
	catch (const std::out_of_range&)
	{
		verify(true
			, "if pos > size(), then throw out_of_range exception");
	}
	catch (...)
	{
		verify(false, "sv.substr(pos, n), throw an unexpected exception.");
	}

	//compare 
	string_view s1{ "abcde" };
	string_view s2{ "ABCDE" };
	string_view s3{ "abc" };

	verify(s1.compare(s2) > 0, "s1.compare(s2)");
	verify(s2.compare(s1) < 0, "s2.compare(s1)");
	verify(s1.compare(0, 3, s3) == 0, "s1.compare(0, 3, s2)");
	verify(s2.compare(0, 3, s3) < 0, "s1.compare(0, 3, s2)");
	verify(s3.compare(s1) < 0, "s3.compare(s1) < 0");
	verify(s3.compare(s2) > 0, "s3.compare(s2) > 0");
	verify(s1.compare(1, 1, s2, 1, 1) > 0, "s1.compare(1, 1, s2, 1, 1)");
	verify(s2.compare(1, 1, s3, 1, 1) < 0, "s2.compare(1, 1, s3, 1, 1)");
	verify(s3.compare(1, 1, s1, 1, 1) == 0, "s3.compare(1, 1, s1, 1, 1)");

	//compare empty string_view
	verify(empty_sv.compare(s1) < 0, "empty_sv.compare(s1)");
	verify(s3.compare(empty_sv) > 0, "s3.compare(empty_sv)");

	str = "ABC";
	verify(s1.compare(str) > 0, "s1.compare(str)");
	verify(s2.compare(0, 3, str) == 0, "empty_sv.compare(0, 5, str)");
	verify(s3.compare(str) > 0, "empty_sv.compare(0, 5, str, 5)");

	//8. Searching
	sv = "ABCDEFGHABCDEFGH";
	
	verify(sv.find("CD") == 2, "sv.find(str)");
	verify(sv.rfind("CD") == 10, "sv.rfind(str)");
	verify(sv.find("") == 0, "sv.find(str)");
	verify(sv.rfind("") == sv.size(), "sv.rfind(str)");
	verify(sv.find("BG") == string_view::npos, "sv.find(str)");
	verify(sv.rfind("BG") == string_view::npos, "sv.rfind(str)");

	verify(sv.find_first_of("CD") == 2, "sv.find_first_of(str)");
	verify(sv.find_last_of("CD") == 11, "sv.find_last_of(str)");
	verify(sv.find_first_of("") == string_view::npos, "sv.find_first_of(str)");
	verify(sv.find_last_of("") == string_view::npos, "sv.find_last_of(str)");
	verify(sv.find_first_of("BG") == 1, "sv.find_first_of(str)");
	verify(sv.find_last_of("BG") == 14, "sv.find_last_of(str)");

	verify(sv.find_first_not_of("ABC") == 3, "sv.find_first_not_of(str)");
	verify(sv.find_last_not_of("FGH") == 12, "sv.find_last_not_of(str)");
	verify(sv.find_first_not_of("BCD") == 0, "sv.find_first_not_of(str)");
	verify(sv.find_last_not_of("EFG") == 15, "sv.find_last_not_of(str)");
	verify(sv.find_first_not_of("") == 0, "sv.find_first_not_of(str)");
	verify(sv.find_last_not_of("") == 15, "sv.find_last_not_of(str)");
}