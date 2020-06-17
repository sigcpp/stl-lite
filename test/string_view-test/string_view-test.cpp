/*
* string_view-test.cpp
* Ray Chen, Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Unit tests for string_view
* - see C++17 [string.view]
* - https://timsong-cpp.github.io/cppwp/n4659/string.view
*/

#include <stdexcept>

#include "../verifiers.h"

void string_view_test()
{
	using std::string_view;

	//1. Construction and assignment
	//2. Capacity

	//default constructor. Constructs an empty string_view. 
	string_view empty_sv;
	is_zero(empty_sv.size(), "empty_sv.size()");
	is_true(empty_sv.length() == empty_sv.size(), "empty_sv.length() == empty_sv.size()");
	is_true(empty_sv.empty(), "empty_sv.empty()");
	is_true(empty_sv.data() == nullptr, "empty_sv.data() == nullptr");

	//constructs a string_view with const char[]
	const char c_hw[]{ "hello, world!" };
	string_view sv{ c_hw };
	is_true(sv.size() == 13, "sv.size()");
	is_true(sv.length() == sv.size(), "sv.length() == sv.size()");
	is_false(sv.empty(), "!sv.empty()");
	is_true(sv.data() == c_hw, "sv.data() == c_hw");

	//constructs a view of the first 5 characters of the C-string
	string_view f_sv{ c_hw, 5 };
	is_true(f_sv.size() == 5, "f_sv.size()");
	is_true(f_sv.length() == f_sv.size(), "f_sv.length() == f_sv.size()");
	is_false(f_sv.empty(), "!f_sv.empty()");
	is_true(f_sv.data() == c_hw, "f_sv.data() == c_hw");

	//constructs a view of the first 5 characters of the character array
	const char a_hw[]{ 'h','e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
	string_view sv_a{ a_hw, 5 };
	is_true(sv_a.size() == 5, "sv_a.size()");
	is_true(sv_a.length() == sv_a.size(), "sv_a.length() == sv_a.size()");
	is_false(sv_a.empty(), "!sv_a.empty()");
	is_true(sv_a.data() == a_hw, "sv_a.data() == a_hw");

	//copy constructor
	string_view c_sv{ f_sv };
	is_true(c_sv.size() == f_sv.size(), "c_sv.size() == f_sv.size()");
	is_true(c_sv.length() == c_sv.size(), "c_sv.length() == c_sv.size()");
	is_true(c_sv.empty() == f_sv.empty(), "c_sv.empty() == f_sv.empty()");
	is_true(c_sv.data() == c_hw, "c_sv.data() == c_hw");

	//assignment
	string_view a_sv = c_sv;

	is_true(a_sv.size() == c_sv.size(), "a_sv.size() == c_sv.size()");
	is_true(a_sv.length() == a_sv.size(), "a_sv.length() == a_sv.size()");
	is_true(a_sv.empty() == c_sv.empty(), "a_sv.empty() == c_sv.empty()");
	is_true(a_sv.data() == c_sv.data(), "a_sv.data() == c_sv.data()");

	//3. Non-member comparison functions
	string_view sv_empty;
	string_view sv_A1("A");
	string_view sv_a1("a");
	string_view sv_A2("A");
	char zA[]{ "A" };
	char zB[]{ "B" };

	is_false(sv_empty == sv_A1, "sv_empty == sv_A1");
	is_false(sv_A1 == sv_empty, "sv_A1 == sv_empty");
	is_false(sv_empty == zA, "sv_empty == zA");
	is_false(zA == sv_empty, "zA == sv_empty");
	is_false(sv_A1 == sv_a1, "sv_A1 == sv_a1");
	is_true(sv_A1 == sv_A2, "sv_A1 == sv_A2");
	is_true(sv_A2 == sv_A1, "sv_A2 == sv_A1");
	is_true(sv_A1 == zA, "sv_A1 == zA");
	is_true(sv_a1 != zB, "sv_a1 != zB");
	is_false(sv_A1 != sv_A2, "sv_A1 != sv_A2");
	is_true(sv_A1 != sv_a1, "sv_A1 != sv_a1");
	is_false(sv_A1 != zA, "!(sv_A1 != zA)");
	is_true(sv_A1 != zB, "sv_A1 != zB");
	is_true(sv_empty != sv_A1, "sv_empty != sv_A1");
	is_true(sv_A1 != sv_empty, "sv_A1 != sv_empty");
	is_true(sv_empty != zA, "sv_empty != zA");
	is_true(zA != sv_empty, "zA != sv_empty");

	is_false(sv_empty > sv_A1, "sv_empty > sv_A1");
	is_true(sv_empty < sv_A1, "sv_empty < sv_A1");
	is_false(sv_empty > zA, "sv_empty > zA");
	is_true(sv_empty < zA, "sv_empty < zA");
	is_false(sv_A1 > sv_a1, "sv_A1 > sv_a1");
	is_true(sv_A1 < sv_a1, "sv_A1 < sv_a1");
	is_true(sv_a1 > sv_A1, "sv_a1 > sv_A1");
	is_false(sv_a1 < sv_A1, "sv_a1 < sv_A1");
	is_false(sv_A1 > sv_A2, "sv_A1 > sv_A2");
	is_false(sv_A1 < sv_A2, "sv_A1 < sv_A2");
	is_false(sv_A1 > zA, "sv_A1 > zA");
	is_false(sv_A1 < zA, "sv_A1 < zA");
	is_false(sv_A1 > zB, "sv_A1 > zB");
	is_true(sv_A1 < zB, "sv_A1 < zB");

	is_false(sv_empty >= sv_A1, "sv_empty >= sv_A1");
	is_true(sv_empty <= sv_A1, "sv_empty <= sv_A1");
	is_false(sv_empty >= zA, "sv_empty >= zA");
	is_true(sv_empty <= zA, "sv_empty <= zA");
	is_false(sv_A1 >= sv_a1, "sv_A1 >= sv_a1");
	is_true(sv_A1 <= sv_a1, "sv_A1 <= sv_a1");
	is_true(sv_a1 >= sv_A1, "sv_a1 >= sv_A1");
	is_false(sv_a1 <= sv_A1, "sv_a1 <= sv_A1");
	is_true(sv_A1 >= sv_A2, "sv_A1 >= sv_A2");
	is_true(sv_A1 <= sv_A2, "sv_A1 <= sv_A2");
	is_true(sv_A1 >= zA, "sv_A1 >= zA");
	is_true(sv_A1 <= zA, "sv_A1 <= zA");
	is_false(sv_A1 >= zB, "sv_A1 >= zB");
	is_true(sv_A1 <= zB, "sv_A1 <= zB");


	//4. Element data and access

	//c_hw is defined earlier as const C-string "hello, world!"
	//constructor and data test
	string_view sv_1(c_hw);
	is_true(sv_1[0] == c_hw[0], "sv_1[0]");
	is_true(sv_1[3] == c_hw[3], "sv_1[3]");
	is_true(sv_1[6] == c_hw[6], "sv_1[6]");
	is_true(sv_1[9] == c_hw[9], "sv_1[9]");
	is_true(sv_1[12] == c_hw[12], "sv_1[12]");

	is_true(sv_1.at(0) == c_hw[0], "sv_1.at(0)");
	is_true(sv_1.at(3) == c_hw[3], "sv_1.at(3)");
	is_true(sv_1.at(6) == c_hw[6], "sv_1.at(6)");
	is_true(sv_1.at(9) == c_hw[9], "sv_1.at(9)");
	is_true(sv_1.at(12) == c_hw[12], "sv_1.at(12)");

	is_true(sv_1.front() == c_hw[0], "sv_1.front()");
	is_true(sv_1.back() == c_hw[sv_1.size() - 1], "sv_1.back()");

	//constructs a view of the first count characters of the character array and data test
	const char* str_2 = "fixed length";
	string_view sv_2(str_2, 5);

	is_true(sv_2[0] == str_2[0], "sv_2[0]");
	is_true(sv_2[2] == str_2[2], "sv_2[2]");
	is_true(sv_2[4] == str_2[4], "sv_2[4]");

	is_true(sv_2.at(0) == str_2[0], "sv_2.at(0)");
	is_true(sv_2.at(2) == str_2[2], "sv_2.at(2)");
	is_true(sv_2.at(4) == str_2[4], "sv_2.at(4)");

	is_true(sv_2.front() == str_2[0], "sv_2.front()");
	is_true(sv_2.back() == str_2[sv_2.size() - 1], "sv_2.back()");


	//member access
	char c_member_access_data[]{ "nice is good" };
	string_view sv_3(c_member_access_data);
	size_t size = sv_3.size();

	//front and back
	is_true(sv_3.front() == c_member_access_data[0], "sv_3.front()");
	is_true(sv_3.back() == c_member_access_data[size-1], "sv_3.back()");

	//subscript operator
	bool access_test{ true };
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_3[i] == c_member_access_data[i]);
	is_true(access_test, "sv_3 subscript");

	//at
	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_3.at(i) == c_member_access_data[i]);
	is_true(access_test, "sv_3.at");


	//5. Iterator support
	//forward iterators
	char sExpected[]{ "support" };
	string_view sv_iter(sExpected);

	size = sizeof(sExpected) - 1;
	bool iteratorTest{ true };

	//forward iterator
	auto it = sv_iter.begin(), endIt = sv_iter.end();
	for (size_t i = 0; i < size && iteratorTest; ++i, ++it)
		iteratorTest = (it != endIt && *it == sExpected[i]);

	is_true(iteratorTest, "forward iterator order and content");
	is_true(it == endIt, "forward iterator termination");

	//const forward iterator
	iteratorTest = true;
	it = sv_iter.cbegin();
	endIt = sv_iter.cend();
	for (size_t i = 0; i < size && iteratorTest; ++i, ++it)
		iteratorTest = (it != endIt && *it == sExpected[i]);

	is_true(iteratorTest, "forward const iterator order and content");
	is_true(it == endIt, "forward const iterator termination");

	//reverse iterator
	string_view sv_reverse("987654321");
	char svrExpected[]{ "123456789" };

	size = sizeof(svrExpected) - 1;
	iteratorTest = true;
	auto rit = sv_reverse.rbegin(), rendIt = sv_reverse.rend();
	for (size_t i = 0; i < size && iteratorTest; ++i, ++rit)
		iteratorTest = (rit != rendIt && *rit == svrExpected[i]);

	is_true(iteratorTest, "reverse iterator order and content");
	is_true(rit == rendIt, "reverse iterator termination");

	//const reverse iterator
	iteratorTest = true;
	rit = sv_reverse.crbegin();
	rendIt = sv_reverse.crend();
	for (size_t i = 0; i < size && iteratorTest; ++i, ++rit)
		iteratorTest = (rit != rendIt && *rit == svrExpected[i]);

	is_true(iteratorTest, "reverse const iterator order and content");
	is_true(rit == rendIt, "reverse const iterator termination");

	//iterator on empty string_view: the loop body should not execute
	iteratorTest = true;
	for (const auto emp : empty_sv)
		iteratorTest = false;
	is_true(iteratorTest, "iterator on empty array");


	//6. Modifiers
	const char* data_expected{ sv.data() + 1 };
	size_t size_expected{ sv.size() - 1 };

	sv.remove_prefix(1);
	is_true(sv.data() == data_expected, "sv.remove_prefix().data()");
	is_true(sv.size() == size_expected, "sv.remove_prefix().size()");

	data_expected = sv.data();
	size_expected--;
	sv.remove_suffix(1);
	is_true(sv.data() == data_expected, "sv.remove_suffix().data()");
	is_true(sv.size() == size_expected, "sv.remove_suffix().size()");

	
	//swap
	char c_lunch[]{ "lunch!" };
	char c_dinner[]{ "dinner" };
	string_view o_sv(c_lunch);
	string_view other_sv(c_dinner);

	o_sv.swap(other_sv);
	bool swap_test = o_sv.data() == c_dinner && o_sv.size() == sizeof(c_dinner) - 1 && 
		other_sv.data() == c_lunch && other_sv.size() == sizeof(c_lunch) - 1;

	is_true(swap_test, "o_sv.swap(other_sv)");


	//7. String operations
	//copy
	char str_copy[]{ "chips" };
	char copy_expected[]{ "hells" };
	size_t rlen, pos{ 0 };
	size_t n = 4;

	//f_sv is defined earlier as string_view "hello"
	rlen = f_sv.copy(str_copy, n, pos);
	//copy(CharT * dest, size_type count, size_type pos = 0)
	//Copies the substring [pos, pos + rcount) to the character array pointed to by dest, 
	//where rcount is the smaller of count and size() - pos.
	is_true(rlen == (n < f_sv.size() - pos) ? n : f_sv.size() - pos, "f_sv.copy(str_copy, n, pos)");
	is_zero(strcmp(str_copy, copy_expected), "str_copy == copy_expected");
	try {
		//throw std::out_of_range exception if pos > size() 
		f_sv.copy(str_copy, n, f_sv.size() + 1);
		is_true(false, "f_sv.copy(str_copy, n, pos), should not be executed.");
	}
	catch (const std::out_of_range&) {
		is_true(true, "if pos > size(), then throw out_of_range exception");
	}
	catch (...) {
		is_true(false, "Throw an unexpected exception.");
	}

	//substr
	sv = "something";
	pos = 4;
	n = 5;
	string_view sub_expected{ "thing" };

	string_view sub_sv(sv.substr(pos, n));
	//substr(size_type pos = 0, size_type count = npos)
	//Returns a view of the substring [pos, pos + rcount), 
	//where rcount is the smaller of count and size() - pos
	is_true(sub_sv.size() == (n < sv.size() - pos) ? n : sv.size() - pos, "sv.copy(pos, n).size");
	is_true(sub_sv.data() == sv.data() + pos, "sv.copy(pos, n).data");
	is_true(sub_sv == sub_expected, "");
	try {
		//throw std::out_of_range exception if pos > size() 
		pos = sv.size() + 1;
		sub_sv = sv.substr(pos, n);
		is_true(false, "sv.substr(pos, n), should not be executed.");
	}
	catch (const std::out_of_range&) {
		is_true(true, "if pos > size(), then throw out_of_range exception");
	}
	catch (...) {
		is_true(false, "sv.substr(pos, n), throw an unexpected exception.");
	}

	//compare 
	string_view s1("abcde");
	string_view s2("ABCDE");
	string_view s3("abc");

	is_positive(s1.compare(s2), "s1.compare(s2)");
	is_negative(s2.compare(s1), "s2.compare(s1)");
	is_zero(s1.compare(0, 3, s3), "s1.compare(0, 3, s2)");
	is_negative(s2.compare(0, 3, s3), "s1.compare(0, 3, s2)");
	is_negative(s3.compare(s1), "s3.compare(s1) < 0");
	is_positive(s3.compare(s2), "s3.compare(s2) > 0");
	is_positive(s1.compare(1, 1, s2, 1, 1), "s1.compare(1, 1, s2, 1, 1)");
	is_negative(s2.compare(1, 1, s3, 1, 1), "s2.compare(1, 1, s3, 1, 1)");
	is_zero(s3.compare(1, 1, s1, 1, 1), "s3.compare(1, 1, s1, 1, 1)");

	//compare empty string_view
	is_negative(empty_sv.compare(s1), "empty_sv.compare(s1)");
	is_positive(s3.compare(empty_sv), "s3.compare(empty_sv)");

	const char* str = "ABC";
	is_positive(s1.compare(str), "s1.compare(str)");
	is_zero(s2.compare(0, 3, str), "empty_sv.compare(0, 5, str)");
	is_positive(s3.compare(str), "empty_sv.compare(0, 5, str, 5)");


	//8. Searching
	//    0         1 
	//    01234567890123456
	sv = "access to success";
	is_true(sv.find("ce") == 2, "sv.find(str)");
	is_true(sv.rfind("ce") == 13, "sv.rfind(str)");
	is_zero(sv.find(""), "sv.find(str)");
	is_true(sv.rfind("") == sv.size(), "sv.rfind(str)");
	is_true(sv.find("TO") == string_view::npos, "sv.find(str)");
	is_true(sv.rfind("as") == string_view::npos, "sv.rfind(str)");

	is_true(sv.find_first_of("cess") == 1, "sv.find_first_of(str)");
	is_true(sv.find_last_of("cce") == 14, "sv.find_last_of(str)");
	is_true(sv.find_first_of("") == string_view::npos, "sv.find_first_of(str)");
	is_true(sv.find_last_of("") == string_view::npos, "sv.find_last_of(str)");
	is_true(sv.find_first_of("sT") == 4, "sv.find_first_of(str)");
	is_true(sv.find_last_of("wc") == 13, "sv.find_last_of(str)");

	is_true(sv.find_first_not_of("ac") == 3, "sv.find_first_not_of(str)");
	is_true(sv.find_last_not_of("cess") == 11, "sv.find_last_not_of(str)");
	is_zero(sv.find_first_not_of("success"), "sv.find_first_not_of(str)");
	is_true(sv.find_last_not_of("success") == 9, "sv.find_last_not_of(str)");
	is_zero(sv.find_first_not_of(""), "sv.find_first_not_of(str)");
	is_true(sv.find_last_not_of("") == 16, "sv.find_last_not_of(str)");
}
