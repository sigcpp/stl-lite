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

#include <stdexcept>
#include "tester.h"

void runTests()
{
	using std::string_view;

	//1. Construction and assignment
	//2. Capacity

	// Default constructor. Constructs an empty string_view. 
	string_view empty_sv;
	verify(empty_sv.size() == 0, "empty_sv.size()");
	verify(empty_sv.length() == empty_sv.size(), "empty_sv.length()");
	verify(empty_sv.empty(), "empty_sv.empty()");

	//constructs a string_view with const char
	string_view sv("hello, world!");
	verify(sv.size() == 13, "sv.size()");
	verify(sv.length() == sv.size(), "sv.length()");
	verify(!sv.empty(), "!sv.empty()");

	//Constructs a view of the first 5 characters of the character array
	string_view f_sv("hello, world!", 5);
	verify(f_sv.size() == 5, "f_sv.size()");
	verify(f_sv.length() == f_sv.size(), "f_sv.length()");
	verify(!f_sv.empty(), "!f_sv.empty()");

	//Copy constructor. Constructs a view of the same content as other.
	string_view r_sv(f_sv);
	verify(r_sv.size() == f_sv.size(), "r_sv.size()");
	verify(r_sv.length() == r_sv.size(), "r_sv.length()");
	verify(r_sv.empty() == f_sv.empty(), "r_sv.empty() == f_sv.empty()");

	//Assigns a view(replaces the view with that of view).
	string_view a_sv = r_sv;
	verify(a_sv.size() == r_sv.size(), "a_sv.size()");
	verify(a_sv.length() == a_sv.size(), "a_sv.length()");
	verify(a_sv.empty() == r_sv.empty(), "a_sv.empty() == sv.empty()");

	//3. Non-member comparison functions
	string_view s("A");
	string_view t("a");
	string_view p("A");
	char cs1[]{ "A" };
	char cs2[]{ "B" };

	verify(!(s == t), "s==t");
	verify(s == p, "s==p");
	verify(p == s, "p==s");
	verify(s == cs1, "s == cs1");
	verify(t != cs2, "t != cs2");
	verify(!(s != p), "s != p");
	verify(s != t, "s != t");
	verify(!(s != cs1), "!(s != cs1)");
	verify(s != cs2, "s != cs2");

	verify(!(s > t), "s > t");
	verify(s < t, "s > t");
	verify(t > s, "t > s");
	verify(!(t < s), "!(t < s)");
	verify(!(s > p), "!(s > p)");
	verify(!(s < p), "!(s < p)");
	verify(!(s > cs1), "!(s > cs1)");
	verify(!(s < cs1), "!(s < cs1)");
	verify(!(s > cs2), "!(s > cs2)");
	verify(s < cs2, "s < cs2");

	verify(!(s >= t), "s > t");
	verify(s <= t, "s > t");
	verify(t >= s, "t > s");
	verify(!(t <= s), "t > s");
	verify(s >= p, "s > t");
	verify(s <= p, "s > t");

	verify(s >= cs1, "s > t");
	verify(s <= cs1, "s > t");
	verify(!(s >= cs2), "s > t");
	verify(s <= cs2, "s > t");

	//4. Element data and access
	verify(empty_sv.data() == nullptr, "empty_sv.data()");

	const char* str_1 = "hello, world!";
	string_view sv_1(str_1);
	verify(sv_1.data() == str_1, "sv_1.data()");
	//normal
	verify(sv_1[0] == str_1[0], "sv_1[0]");
	verify(sv_1[3] == str_1[3], "sv_1[3]");
	verify(sv_1[6] == str_1[6], "sv_1[6]");
	verify(sv_1[9] == str_1[9], "sv_1[9]");
	verify(sv_1[12] == str_1[12], "sv_1[12]");

	verify(sv_1.at(0) == str_1[0], "sv_1.at(0)");
	verify(sv_1.at(3) == str_1[3], "sv_1.at(3)");
	verify(sv_1.at(6) == str_1[6], "sv_1.at(6)");
	verify(sv_1.at(9) == str_1[9], "sv_1.at(9)");
	verify(sv_1.at(12) == str_1[12], "sv_1.at(12)");

	verify(sv_1.front() == str_1[0], "sv_1.front()");
	verify(sv_1.back() == str_1[sv_1.size() - 1], "sv_1.back()");

	const char* str_2 = "fixed length";
	string_view sv_2(str_2, 5);
	verify(sv_2.data() == str_2, "sv_2.data()");
	//fixed-length
	verify(sv_2[0] == str_2[0], "sv_2[0]");
	verify(sv_2[2] == str_2[2], "sv_2[2]");
	verify(sv_2[4] == str_2[4], "sv_2[4]");

	verify(sv_2.at(0) == str_2[0], "sv_2.at(0)");
	verify(sv_2.at(2) == str_2[2], "sv_2.at(2)");
	verify(sv_2.at(4) == str_2[4], "sv_2.at(4)");

	verify(sv_2.front() == str_2[0], "sv_2.front()");
	verify(sv_2.back() == str_2[sv_2.size() - 1], "sv_2.back()");

	string_view sv_3(str_2);
	verify(sv_3.data() == str_2, "sv_3.data()");
	//reference
	verify(sv_3[0] == str_2[0], "sv_3[0]");
	verify(sv_3[3] == str_2[3], "sv_3[3]");
	verify(sv_3[6] == str_2[6], "sv_3[6]");
	verify(sv_3[9] == str_2[9], "sv_3[9]");
	verify(sv_3[11] == str_2[11], "sv_3[11]");

	verify(sv_3.at(0) == str_2[0], "sv_3.at(0)");
	verify(sv_3.at(3) == str_2[3], "sv_3.at(3)");
	verify(sv_3.at(6) == str_2[6], "sv_3.at(6)");
	verify(sv_3.at(9) == str_2[9], "sv_3.at(9)");
	verify(sv_3.at(11) == str_2[11], "sv_3.at(11)");

	verify(sv_3.front() == str_2[0], "sv_3.front()");
	verify(sv_3.back() == str_2[sv_3.size() - 1], "sv_3.back()");

	string_view sv_4 = str_2;
	verify(sv_4.data() == str_2, "sv_4.data()");
	//assignment
	verify(sv_4[0] == str_2[0], "sv_4[0]");
	verify(sv_4[3] == str_2[3], "sv_4[3]");
	verify(sv_4[6] == str_2[6], "sv_4[6]");
	verify(sv_4[9] == str_2[9], "sv_4[9]");
	verify(sv_4[11] == str_2[11], "sv_4[11]");

	verify(sv_4.at(0) == str_2[0], "sv_4.at(0)");
	verify(sv_4.at(3) == str_2[3], "sv_4.at(3)");
	verify(sv_4.at(6) == str_2[6], "sv_4.at(6)");
	verify(sv_4.at(9) == str_2[9], "sv_4.at(9)");
	verify(sv_4.at(11) == str_2[11], "sv_4.at(11)");

	verify(sv_4.front() == str_2[0], "sv_4.front()");
	verify(sv_4.back() == str_2[sv_4.size() - 1], "sv_4.back()");

	//5. Iterator support
	//forward iterators
	char sExpected[]{ "support" };
	string_view sv_iter(sExpected);

	bool iteratorTest{ true };
	size_t size = sizeof(sExpected) - 1, i = 0;
	auto it = sv_iter.begin(), endIt = sv_iter.end();
	for (; i < size && iteratorTest; ++i, ++it)
		iteratorTest = (it != endIt && *it == sExpected[i]);

	verify(iteratorTest, "forward iterator order and content");
	verify(it == endIt, "forward iterator termination");

	iteratorTest =true;
	i = 0;
	it = sv_iter.cbegin();
	endIt = sv_iter.cend();
	for (; i < size && iteratorTest; ++i, ++it)
		iteratorTest = (it != endIt && *it == sExpected[i]);

	verify(iteratorTest, "forward const iterator order and content");
	verify(it == endIt, "forward const iterator termination");

	//reverse iterators
	char svrExpected[]{ "123456789" };
	string_view sv_reverse("987654321");

	iteratorTest = true;
	size = sizeof(svrExpected) - 1;
	i = 0;
	auto rit = sv_reverse.rbegin(), rendIt = sv_reverse.rend();
	for (; i < size && iteratorTest; ++i, ++rit)
		iteratorTest = (rit != rendIt && *rit == svrExpected[i]);

	verify(iteratorTest, "reverse iterator order and content");
	verify(rit == rendIt, "reverse iterator termination");

	iteratorTest = true;
	i = 0;
	rit = sv_reverse.crbegin();
	rendIt = sv_reverse.crend();
	for (; i < size && iteratorTest; ++i, ++rit)
		iteratorTest = (rit != rendIt && *rit == svrExpected[i]);

	verify(iteratorTest, "reverse iterator order and content");
	verify(rit == rendIt, "reverse iterator termination");

	//iterator on empty string_view: the loop body should not execute
	iteratorTest = true;
	for (const auto emp : empty_sv)
		iteratorTest = false;
	verify(iteratorTest, "iterator on empty array");

	//6. Modifiers
	const char* sv_data{ sv.data() };
	size_t sv_size{ sv.size() };
	size_t n{ 1 };

	sv.remove_prefix(n);
	sv_size -= n;
	verify((sv.data() == sv_data + 1 && sv.size() == sv_size)
		, "sv.remove_prefix()");
	sv_data = sv.data();

	sv.remove_suffix(n);
	sv_size -= n;
	verify((sv.data() == sv_data && sv.size() == sv_size)
		, "sv.remove_suffix()");

	//swap(exchanges the values of *this and s.)
	string_view o_sv("lunch!");
	string_view s_sv("dinner");
	char owExpected[]{ "lunch!" };
	char swExpected[]{ "dinner" };
	o_sv.swap(s_sv);
	bool swapTest = true;
	for (std::size_t idx = 0; idx < o_sv.size() && swapTest; ++idx)
		swapTest = o_sv[idx] == swExpected[idx] && s_sv[idx] == owExpected[idx];
	verify(swapTest, "o_sv.swap(s_sv)");

	//7. String operations
	//copy
	char str_copy[]{ "chips" };
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
	string_view sub_sv(sv.substr(pos, n));

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
	string_view s1("abcde");
	string_view s2("ABCDE");
	string_view s3("abc");

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

	const char* str = "ABC";
	verify(s1.compare(str) > 0, "s1.compare(str)");
	verify(s2.compare(0, 3, str) == 0, "empty_sv.compare(0, 5, str)");
	verify(s3.compare(str) > 0, "empty_sv.compare(0, 5, str, 5)");

	//8. Searching
	//    0         1 
	//    01234567890123456
	sv = "access to success";
	verify(sv.find("ce") == 2, "sv.find(str)");
	verify(sv.rfind("ce") == 13, "sv.rfind(str)");
	verify(sv.find("") == 0, "sv.find(str)");
	verify(sv.rfind("") == sv.size(), "sv.rfind(str)");
	verify(sv.find("TO") == string_view::npos, "sv.find(str)");
	verify(sv.rfind("as") == string_view::npos, "sv.rfind(str)");

	verify(sv.find_first_of("cess") == 1, "sv.find_first_of(str)");
	verify(sv.find_last_of("cce") == 14, "sv.find_last_of(str)");
	verify(sv.find_first_of("") == string_view::npos, "sv.find_first_of(str)");
	verify(sv.find_last_of("") == string_view::npos, "sv.find_last_of(str)");
	verify(sv.find_first_of("sT") == 4, "sv.find_first_of(str)");
	verify(sv.find_last_of("wc") == 13, "sv.find_last_of(str)");

	verify(sv.find_first_not_of("ac") == 3, "sv.find_first_not_of(str)");
	verify(sv.find_last_not_of("cess") == 11, "sv.find_last_not_of(str)");
	verify(sv.find_first_not_of("success") == 0, "sv.find_first_not_of(str)");
	verify(sv.find_last_not_of("success") == 9, "sv.find_last_not_of(str)");
	verify(sv.find_first_not_of("") == 0, "sv.find_first_not_of(str)");
	verify(sv.find_last_not_of("") == 16, "sv.find_last_not_of(str)");
}