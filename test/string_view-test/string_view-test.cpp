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

using std::string_view;

void test_ctors_and_assignment();
void test_non_member_comparison();
void test_element_access();
void test_iterator();
void test_modifiers();
void test_operations();
void test_finders();

void string_view_test()
{
	test_ctors_and_assignment();
	test_non_member_comparison();
	test_element_access();
	test_iterator();
	test_modifiers();
	test_operations();
	test_finders();
}


void test_ctors_and_assignment()
{
	//default ctor
	string_view sv_empty;
	is_zero(sv_empty.size(), "sv_empty.size()");
	is_true(sv_empty.length() == sv_empty.size(), "sv_empty.length() == sv_empty.size()");
	is_true(sv_empty.empty(), "sv_empty.empty()");
	is_true(sv_empty.data() == nullptr, "sv_empty.data() == nullptr");

	//custom ctor, C-string
	const char z_hw[]{ "hello, world!" };
	string_view sv{ z_hw };
	is_true(sv.size() == 13, "sv.size()");
	is_true(sv.length() == sv.size(), "sv.length() == sv.size()");
	is_false(sv.empty(), "!sv.empty()");
	is_true(sv.data() == z_hw, "sv.data() == z_hw");

	//custom ctor, first few characters of C-string
	string_view sv_f{ z_hw, 5 };
	is_true(sv_f.size() == 5, "sv_f.size()");
	is_true(sv_f.length() == sv_f.size(), "sv_f.length() == sv_f.size()");
	is_false(sv_f.empty(), "!sv_f.empty()");
	is_true(sv_f.data() == z_hw, "sv_f.data() == z_hw");

	//custom ctor, first few characters of non-C-string char array
	const char a_hw[]{ 'h','e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
	string_view sv_h{ a_hw, 5 };
	is_true(sv_h.size() == 5, "sv_h.size()");
	is_true(sv_h.length() == sv_h.size(), "sv_h.length() == sv_h.size()");
	is_false(sv_h.empty(), "!sv_h.empty()");
	is_true(sv_h.data() == a_hw, "sv_h.data() == a_hw");

	//copy ctor
	string_view sv_c{ sv_f };
	is_true(sv_c.size() == sv_f.size(), "sv_c.size() == sv_f.size()");
	is_true(sv_c.length() == sv_c.size(), "sv_c.length() == sv_c.size()");
	is_true(sv_c.empty() == sv_f.empty(), "sv_c.empty() == sv_f.empty()");
	is_true(sv_c.data() == sv_f.data(), "sv_c.data() == sv_f.data()");

	//assignment
	const char z_h[]{ "hello" };
	string_view sv_a;
	sv_a = z_h;
	is_true(sv_a.size() == sizeof(z_h) - 1, "sv_a.size() == sizeof(z_h) - 1");
	is_true(sv_a.length() == sv_a.size(), "sv_a.length() == sv_a.size()");
	is_false(sv_a.empty(), "sv_a.empty()");
	is_true(sv_a.data() == z_h, "sv_a.data() == sv_c.data()");
}


void test_non_member_comparison()
{
	string_view sv_empty;
	string_view sv_A1{ "A" };
	string_view sv_a1{ "a" };
	string_view sv_A2{ "A" };
	char z_A[]{ "A" };
	char z_B[]{ "B" };

	is_false(sv_empty == sv_A1, "sv_empty == sv_A1");
	is_false(sv_A1 == sv_empty, "sv_A1 == sv_empty");
	is_false(sv_empty == z_A, "sv_empty == z_A");
	is_false(z_A == sv_empty, "z_A == sv_empty");
	is_false(sv_A1 == sv_a1, "sv_A1 == sv_a1");
	is_true(sv_A1 == sv_A2, "sv_A1 == sv_A2");
	is_true(sv_A2 == sv_A1, "sv_A2 == sv_A1");
	is_true(sv_A1 == z_A, "sv_A1 == z_A");

	is_true(sv_a1 != z_B, "sv_a1 != z_B");
	is_false(sv_A1 != sv_A2, "sv_A1 != sv_A2");
	is_true(sv_A1 != sv_a1, "sv_A1 != sv_a1");
	is_false(sv_A1 != z_A, "!(sv_A1 != z_A)");
	is_true(sv_A1 != z_B, "sv_A1 != z_B");
	is_true(sv_empty != sv_A1, "sv_empty != sv_A1");
	is_true(sv_A1 != sv_empty, "sv_A1 != sv_empty");
	is_true(sv_empty != z_A, "sv_empty != z_A");
	is_true(z_A != sv_empty, "z_A != sv_empty");

	is_false(sv_empty > sv_A1, "sv_empty > sv_A1");
	is_false(sv_empty > z_A, "sv_empty > z_A");
	is_false(sv_A1 > sv_a1, "sv_A1 > sv_a1");
	is_true(sv_a1 > sv_A1, "sv_a1 > sv_A1");
	is_false(sv_A1 > sv_A2, "sv_A1 > sv_A2");
	is_false(sv_A1 > z_A, "sv_A1 > z_A");
	is_false(sv_A1 > z_B, "sv_A1 > z_B");

	is_true(sv_empty < sv_A1, "sv_empty < sv_A1");
	is_true(sv_empty < z_A, "sv_empty < z_A");
	is_true(sv_A1 < sv_a1, "sv_A1 < sv_a1");
	is_false(sv_a1 < sv_A1, "sv_a1 < sv_A1");
	is_false(sv_A1 < sv_A2, "sv_A1 < sv_A2");
	is_false(sv_A1 < z_A, "sv_A1 < z_A");
	is_true(sv_A1 < z_B, "sv_A1 < z_B");

	is_false(sv_empty >= sv_A1, "sv_empty >= sv_A1");
	is_false(sv_empty >= z_A, "sv_empty >= z_A");
	is_false(sv_A1 >= sv_a1, "sv_A1 >= sv_a1");
	is_true(sv_a1 >= sv_A1, "sv_a1 >= sv_A1");
	is_true(sv_A1 >= sv_A2, "sv_A1 >= sv_A2");
	is_true(sv_A1 >= z_A, "sv_A1 >= z_A");
	is_false(sv_A1 >= z_B, "sv_A1 >= z_B");

	is_true(sv_empty <= sv_A1, "sv_empty <= sv_A1");
	is_true(sv_empty <= z_A, "sv_empty <= z_A");
	is_true(sv_A1 <= sv_a1, "sv_A1 <= sv_a1");
	is_false(sv_a1 <= sv_A1, "sv_a1 <= sv_A1");
	is_true(sv_A1 <= sv_A2, "sv_A1 <= sv_A2");
	is_true(sv_A1 <= z_A, "sv_A1 <= z_A");
	is_true(sv_A1 <= z_B, "sv_A1 <= z_B");
}


void test_element_access()
{
	char z_member_access_data[]{ "nice is good" };
	string_view sv{ z_member_access_data };
	size_t size{ sv.size() };

	//front and back
	is_true(sv.front() == z_member_access_data[0], "sv.front()");
	is_true(sv.back() == z_member_access_data[size - 1], "sv.back()");

	//subscript operator
	bool access_test{ true };
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv[i] == z_member_access_data[i]);
	is_true(access_test, "sv subscript");

	//at
	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv.at(i) == z_member_access_data[i]);
	is_true(access_test, "sv.at");

	//test pos >= sv.size() case
	try {
		//use `static_cast<void>` to ignore the `nodiscard` attribute
		static_cast<void>(sv.at(sv.size() + 1));
		is_true(false, "sv.at(pos) should not be executed.");
	}
	catch (const std::out_of_range&) {
		is_true(true, "Throw an expected exception.");
	}
	catch (...) {
		is_true(false, "Throw an unexpected exception.");
	}
}


void test_iterator_forward();
void test_iterator_reverse();

void test_iterator()
{
	test_iterator_reverse();
	test_iterator_forward();
}


void test_iterator_forward()
{
	char z_expected[]{ "support" };
	string_view sv_iter{ z_expected };

	//non-const forward iterator
	size_t size = sizeof(z_expected) - 1;
	bool iterator_test{ true };
	auto it = sv_iter.begin(), end_it = sv_iter.end();
	for (size_t i = 0; i < size && iterator_test; ++i, ++it)
		iterator_test = (it != end_it && *it == z_expected[i]);

	is_true(iterator_test, "forward iterator order and content");
	is_true(it == end_it, "forward iterator termination");

	//const forward iterator
	iterator_test = true;
	it = sv_iter.cbegin(), end_it = sv_iter.cend();
	for (size_t i = 0; i < size && iterator_test; ++i, ++it)
		iterator_test = (it != end_it && *it == z_expected[i]);

	is_true(iterator_test, "forward const iterator order and content");
	is_true(it == end_it, "forward const iterator termination");
}


void test_iterator_reverse()
{
	string_view sv_reverse{ "987654321" };
	char z_reverse_expected[]{ "123456789" };

	//non-const reverse iterator
	size_t size = sizeof(z_reverse_expected) - 1;
	bool iterator_test{ true };
	auto r_it = sv_reverse.rbegin(), rend_it = sv_reverse.rend();
	for (size_t i = 0; i < size && iterator_test; ++i, ++r_it)
		iterator_test = (r_it != rend_it && *r_it == z_reverse_expected[i]);

	is_true(iterator_test, "reverse iterator order and content");
	is_true(r_it == rend_it, "reverse iterator termination");

	//const reverse iterator
	iterator_test = true;
	r_it = sv_reverse.crbegin(), rend_it = sv_reverse.crend();
	for (size_t i = 0; i < size && iterator_test; ++i, ++r_it)
		iterator_test = (r_it != rend_it && *r_it == z_reverse_expected[i]);

	is_true(iterator_test, "reverse const iterator order and content");
	is_true(r_it == rend_it, "reverse const iterator termination");

	//iterator on empty string_view: the loop body should not execute
	string_view sv_empty;
	iterator_test = true;
	for (const auto emp : sv_empty)
		iterator_test = false;
	is_true(iterator_test, "iterator on empty array");
}


void test_operation_copy();
void test_operation_substr();
void test_operation_compare();

void test_operations()
{
	test_operation_copy();
	test_operation_substr();
	test_operation_compare();
}


void test_operation_copy()
{
	string_view sv_f{ "hello" };
	char z_copy[]{ "chips" };
	size_t n{ 4 }, pos{ 0 };

	//copies the substring [pos, pos + rlen) to the character array pointed to by `z_copy`, 
	//where `rlen` is the smaller of `n` and `size() - pos`.
	size_t rlen = sv_f.copy(z_copy, n, pos);

	//let `len_expected` be the smaller of `n` and `size() - pos`.
	size_t len_expected{ n };
	const char* p_expected{ "hells" };
	if (n > sv_f.size() - pos) {
		len_expected = sv_f.size() - pos;
		p_expected = z_copy;
	}

	is_true(rlen == len_expected, "sv_f.copy(z_copy, n, pos).length");
	is_zero(std::strncmp(z_copy, p_expected, 5), "z_copy == z_copy_expected");

	//test pos >= sv_f.size() case
	try {
		sv_f.copy(z_copy, n, sv_f.size() + 1);
		is_true(false, "sv_f.copy() should not be executed.");
	}
	catch (const std::out_of_range&) {
		is_true(true, "Thrown an expected exception.");
	}
	catch (...) {
		is_true(false, "Throw an unexpected exception.");
	}
}


void test_operation_substr()
{
	string_view sv = "something";
	size_t pos{ 4 };
	size_t n{ 5 };

	//let `len_expected` be the smaller of `n` and `size() - pos`.
	size_t len_expected{ n };
	if (n > sv.size() - pos) {
		len_expected = sv.size() - pos;
	}

	//returns a view of the substring `[pos, pos + n)`
	string_view sub_sv = sv.substr(pos, n);
	is_true(sub_sv.size() == len_expected, "sv.copy(pos, n).size");
	is_true(sub_sv.data() == sv.data() + pos, "sv.copy(pos, n).data");

	string_view sub_expected{ "thing" };
	is_true(sub_sv == sub_expected, "sv.copy(pos, n)");

	//test pos >= sv_f.size() case
	try {
		pos = sv.size() + 1;
		sub_sv = sv.substr(pos, n);
		is_true(false, "sv.substr(), should not be executed.");
	}
	catch (const std::out_of_range&) {
		is_true(true, "Throw an expected exception");
	}
	catch (...) {
		is_true(false, "Throw an unexpected exception.");
	}
}


void test_operation_compare()
{
	string_view sv_a_to_e{ "abcde" };
	string_view sv_A_to_E{ "ABCDE" };
	string_view sv_a_to_c{ "abc" };

	is_positive(sv_a_to_e.compare(sv_A_to_E), "sv_a_to_e.compare(sv_A_to_E)");
	is_negative(sv_A_to_E.compare(sv_a_to_e), "sv_A_to_E.compare(sv_a_to_e)");
	is_zero(sv_a_to_e.compare(0, 3, sv_a_to_c), "sv_a_to_e.compare(0, 3, sv_a_to_c)");
	is_negative(sv_A_to_E.compare(0, 3, sv_a_to_c), "sv_A_to_E.compare(0, 3, sv_a_to_c)");
	is_negative(sv_a_to_c.compare(sv_a_to_e), "sv_a_to_c.compare(sv_a_to_e) < 0");
	is_positive(sv_a_to_c.compare(sv_A_to_E), "sv_a_to_c.compare(sv_A_to_E) > 0");
	is_positive(sv_a_to_e.compare(1, 1, sv_A_to_E, 1, 1), "sv_a_to_e.compare(1, 1, sv_A_to_E, 1, 1)");
	is_negative(sv_A_to_E.compare(1, 1, sv_a_to_c, 1, 1), "sv_A_to_E.compare(1, 1, sv_a_to_c, 1, 1)");
	is_zero(sv_a_to_c.compare(1, 1, sv_a_to_e, 1, 1), "sv_a_to_c.compare(1, 1, sv_a_to_e, 1, 1)");

	//compare empty string_view
	string_view sv_empty;
	is_negative(sv_empty.compare(sv_a_to_e), "sv_empty.compare(sv_a_to_e)");
	is_positive(sv_a_to_c.compare(sv_empty), "sv_a_to_c.compare(sv_empty)");

	const char* p_ABC{ "ABC" };
	is_positive(sv_a_to_e.compare(p_ABC), "sv_a_to_e.compare(a_ABC)");
	is_zero(sv_A_to_E.compare(0, 3, p_ABC), "sv_A_to_E.compare(0, 3, a_ABC)");
	is_positive(sv_a_to_c.compare(p_ABC), "sv_a_to_c.compare(a_ABC)");
}


void test_finders()
{
	//                   0         1 
	//                   01234567890123456
	string_view sv_ats{ "access to success" };
	is_true(sv_ats.find("ce") == 2, "sv_ats.find(str)");
	is_zero(sv_ats.find(""), "sv_ats.find(str)");
	is_true(sv_ats.find("TO") == string_view::npos, "sv_ats.find(str)");

	is_true(sv_ats.rfind("ce") == 13, "sv_ats.rfind(str)");
	is_true(sv_ats.rfind("") == sv_ats.size(), "sv_ats.rfind(str)");
	is_true(sv_ats.rfind("as") == string_view::npos, "sv_ats.rfind(str)");

	is_true(sv_ats.find_first_of("cess") == 1, "sv_ats.find_first_of(str)");
	is_true(sv_ats.find_first_of("") == string_view::npos, "sv_ats.find_first_of(str)");
	is_true(sv_ats.find_first_of("sT") == 4, "sv_ats.find_first_of(str)");

	is_true(sv_ats.find_last_of("cce") == 14, "sv_ats.find_last_of(str)");
	is_true(sv_ats.find_last_of("") == string_view::npos, "sv_ats.find_last_of(str)");
	is_true(sv_ats.find_last_of("wc") == 13, "sv_ats.find_last_of(str)");

	is_true(sv_ats.find_first_not_of("ac") == 3, "sv_ats.find_first_not_of(str)");
	is_zero(sv_ats.find_first_not_of("success"), "sv_ats.find_first_not_of(str)");
	is_zero(sv_ats.find_first_not_of(""), "sv_ats.find_first_not_of(str)");

	is_true(sv_ats.find_last_not_of("cess") == 11, "sv_ats.find_last_not_of(str)");
	is_true(sv_ats.find_last_not_of("success") == 9, "sv_ats.find_last_not_of(str)");
	is_true(sv_ats.find_last_not_of("") == 16, "sv_ats.find_last_not_of(str)");
}


void test_modifiers()
{
	string_view sv{ "hello, world!" };

	const char* data_expected{ sv.data() + 1 };
	size_t size_expected{ sv.size() - 1 };

	//remove prefix
	sv.remove_prefix(1);
	is_true(sv.size() == size_expected, "sv.remove_prefix().size()");
	is_true(sv.data() == data_expected, "sv.remove_prefix().data()");

	size_expected--;

	//remove suffix
	sv.remove_suffix(1);
	is_true(sv.size() == size_expected, "sv.remove_suffix().size()");
	is_true(sv.data() == data_expected, "sv.remove_suffix().data()");

	//swap
	char z_lunch[]{ "lunch!" };
	char z_dinner[]{ "dinner" };
	string_view o_sv{ z_lunch };
	string_view other_sv{ z_dinner };

	o_sv.swap(other_sv);
	bool swap_test = o_sv.data() == z_dinner && o_sv.size() == sizeof(z_dinner) - 1 &&
		other_sv.data() == z_lunch && other_sv.size() == sizeof(z_lunch) - 1;

	is_true(swap_test, "o_sv.swap(other_sv)");
}
