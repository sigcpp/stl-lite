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
	string_view sv_empty;
	is_zero(sv_empty.size(), "sv_empty.size()");
	is_true(sv_empty.length() == sv_empty.size(), "sv_empty.length() == sv_empty.size()");
	is_true(sv_empty.empty(), "sv_empty.empty()");
	is_true(sv_empty.data() == nullptr, "sv_empty.data() == nullptr");

	//constructs a string_view with const char[]
	const char z_hw[]{ "hello, world!" };
	string_view sv{ z_hw };
	is_true(sv.size() == 13, "sv.size()");
	is_true(sv.length() == sv.size(), "sv.length() == sv.size()");
	is_false(sv.empty(), "!sv.empty()");
	is_true(sv.data() == z_hw, "sv.data() == z_hw");

	//constructs a view of the first 5 characters of the C-string
	string_view sv_f{ z_hw, 5 };
	is_true(sv_f.size() == 5, "sv_f.size()");
	is_true(sv_f.length() == sv_f.size(), "sv_f.length() == sv_f.size()");
	is_false(sv_f.empty(), "!sv_f.empty()");
	is_true(sv_f.data() == z_hw, "sv_f.data() == z_hw");

	//constructs a view of the first 5 characters of the character array
	const char a_hw[]{ 'h','e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
	string_view sv_h{ a_hw, 5 };
	is_true(sv_h.size() == 5, "sv_h.size()");
	is_true(sv_h.length() == sv_h.size(), "sv_h.length() == sv_h.size()");
	is_false(sv_h.empty(), "!sv_h.empty()");
	is_true(sv_h.data() == a_hw, "sv_h.data() == a_hw");

	//copy constructor
	string_view sv_c{ sv_f };
	is_true(sv_c.size() == sv_f.size(), "sv_c.size() == sv_f.size()");
	is_true(sv_c.length() == sv_c.size(), "sv_c.length() == sv_c.size()");
	is_true(sv_c.empty() == sv_f.empty(), "sv_c.empty() == sv_f.empty()");
	is_true(sv_c.data() == z_hw, "sv_c.data() == z_hw");

	//assignment
	string_view sv_a = sv_c;
	is_true(sv_a.size() == sv_c.size(), "sv_a.size() == sv_c.size()");
	is_true(sv_a.length() == sv_a.size(), "sv_a.length() == sv_a.size()");
	is_true(sv_a.empty() == sv_c.empty(), "sv_a.empty() == sv_c.empty()");
	is_true(sv_a.data() == sv_c.data(), "sv_a.data() == sv_c.data()");

	//3. Non-member comparison functions
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
	is_true(sv_empty < sv_A1, "sv_empty < sv_A1");
	is_false(sv_empty > z_A, "sv_empty > z_A");
	is_true(sv_empty < z_A, "sv_empty < z_A");
	is_false(sv_A1 > sv_a1, "sv_A1 > sv_a1");
	is_true(sv_A1 < sv_a1, "sv_A1 < sv_a1");
	is_true(sv_a1 > sv_A1, "sv_a1 > sv_A1");
	is_false(sv_a1 < sv_A1, "sv_a1 < sv_A1");
	is_false(sv_A1 > sv_A2, "sv_A1 > sv_A2");
	is_false(sv_A1 < sv_A2, "sv_A1 < sv_A2");
	is_false(sv_A1 > z_A, "sv_A1 > z_A");
	is_false(sv_A1 < z_A, "sv_A1 < z_A");
	is_false(sv_A1 > z_B, "sv_A1 > z_B");
	is_true(sv_A1 < z_B, "sv_A1 < z_B");

	is_false(sv_empty >= sv_A1, "sv_empty >= sv_A1");
	is_true(sv_empty <= sv_A1, "sv_empty <= sv_A1");
	is_false(sv_empty >= z_A, "sv_empty >= z_A");
	is_true(sv_empty <= z_A, "sv_empty <= z_A");
	is_false(sv_A1 >= sv_a1, "sv_A1 >= sv_a1");
	is_true(sv_A1 <= sv_a1, "sv_A1 <= sv_a1");
	is_true(sv_a1 >= sv_A1, "sv_a1 >= sv_A1");
	is_false(sv_a1 <= sv_A1, "sv_a1 <= sv_A1");
	is_true(sv_A1 >= sv_A2, "sv_A1 >= sv_A2");
	is_true(sv_A1 <= sv_A2, "sv_A1 <= sv_A2");
	is_true(sv_A1 >= z_A, "sv_A1 >= z_A");
	is_true(sv_A1 <= z_A, "sv_A1 <= z_A");
	is_false(sv_A1 >= z_B, "sv_A1 >= z_B");
	is_true(sv_A1 <= z_B, "sv_A1 <= z_B");


	//4. Element data and access
	//member access
	char z_member_access_data[]{ "nice is good" };
	string_view sv_1{ z_member_access_data };
	size_t size{ sv_1.size() };

	//front and back
	is_true(sv_1.front() == z_member_access_data[0], "sv_1.front()");
	is_true(sv_1.back() == z_member_access_data[size - 1], "sv_1.back()");

	//subscript operator
	bool access_test{ true };
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_1[i] == z_member_access_data[i]);
	is_true(access_test, "sv_1 subscript");

	//at
	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_1.at(i) == z_member_access_data[i]);
	is_true(access_test, "sv_1.at");

	//constructs a view of the first count characters of the character array
	//z_member_access_data is defined earlier as const C-string "nice is good"
	string_view sv_2{ z_member_access_data, 7 };
	size = 7;

	is_true(sv_2.front() == z_member_access_data[0], "sv_2.front()");
	is_true(sv_2.back() == z_member_access_data[size - 1], "sv_2.back()");

	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_2[i] == z_member_access_data[i]);
	is_true(access_test, "sv_2 subscript");

	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_2.at(i) == z_member_access_data[i]);
	is_true(access_test, "sv_2.at");

	//copy constructor
	//sv_1 is defined earlier as string_view "nice is good"
	string_view sv_3{ sv_1 };
	size = sv_3.size();

	is_true(sv_3.front() == sv_1[0], "sv_3.front()");
	is_true(sv_3.back() == sv_1[size - 1], "sv_3.back()");

	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_3[i] == sv_1[i]);
	is_true(access_test, "sv_3 subscript");

	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_3.at(i) == sv_1[i]);
	is_true(access_test, "sv_3.at");

	//assignment
	//sv_1 is defined earlier as string_view "nice is good"
	string_view sv_4 = sv_1;
	size = sv_4.size();

	is_true(sv_4.front() == sv_1[0], "sv_4.front()");
	is_true(sv_4.back() == sv_1[size - 1], "sv_4.back()");

	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_4[i] == sv_1[i]);
	is_true(access_test, "sv_4 subscript");

	access_test = true;
	for (size_t i = 0; i < size && access_test; ++i)
		access_test = (sv_4.at(i) == sv_1[i]);
	is_true(access_test, "sv_4.at");

	//5. Iterator support
	//forward iterators
	char z_expected[]{ "support" };
	string_view sv_iter{ z_expected };

	size = sizeof(z_expected) - 1;
	bool iterator_test{ true };

	//forward iterator
	auto it = sv_iter.begin(), end_it = sv_iter.end();
	for (size_t i = 0; i < size && iterator_test; ++i, ++it)
		iterator_test = (it != end_it && *it == z_expected[i]);

	is_true(iterator_test, "forward iterator order and content");
	is_true(it == end_it, "forward iterator termination");

	//const forward iterator
	iterator_test = true;
	it = sv_iter.cbegin();
	end_it = sv_iter.cend();
	for (size_t i = 0; i < size && iterator_test; ++i, ++it)
		iterator_test = (it != end_it && *it == z_expected[i]);

	is_true(iterator_test, "forward const iterator order and content");
	is_true(it == end_it, "forward const iterator termination");

	//reverse iterator
	string_view sv_reverse{ "987654321" };
	char c_reverse_expected[]{ "123456789" };

	size = sizeof(c_reverse_expected) - 1;
	iterator_test = true;
	auto r_it = sv_reverse.rbegin(), rend_it = sv_reverse.rend();
	for (size_t i = 0; i < size && iterator_test; ++i, ++r_it)
		iterator_test = (r_it != rend_it && *r_it == c_reverse_expected[i]);

	is_true(iterator_test, "reverse iterator order and content");
	is_true(r_it == rend_it, "reverse iterator termination");

	//const reverse iterator
	iterator_test = true;
	r_it = sv_reverse.crbegin();
	rend_it = sv_reverse.crend();
	for (size_t i = 0; i < size && iterator_test; ++i, ++r_it)
		iterator_test = (r_it != rend_it && *r_it == c_reverse_expected[i]);

	is_true(iterator_test, "reverse const iterator order and content");
	is_true(r_it == rend_it, "reverse const iterator termination");

	//iterator on empty string_view: the loop body should not execute
	iterator_test = true;
	for (const auto emp : sv_empty)
		iterator_test = false;
	is_true(iterator_test, "iterator on empty array");


	//6. Modifiers
	const char* data_expected{ sv.data() + 1 };
	size_t size_expected{ sv.size() - 1 };

	sv.remove_prefix(1);
	is_true(sv.size() == size_expected, "sv.remove_prefix().size()");
	is_true(sv.data() == data_expected, "sv.remove_prefix().data()");

	data_expected = sv.data();
	size_expected--;
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


	//7. String operations
	//copy
	char z_copy[]{ "chips" };
	char z_copy_expected[]{ "hells" };
	size_t rlen, pos{ 0 };
	size_t n = 4;

	size_t len_expected{ n };
	char* p{ z_copy_expected };

	//sv_f is defined earlier as string_view "hello"
	rlen = sv_f.copy(z_copy, n, pos);
	//copy(CharT * dest, size_type count, size_type pos = 0)
	//Copies the substring [pos, pos + rcount) to the character array pointed to by dest, 
	//where rcount is the smaller of count and size() - pos.
	if (n >= sv_f.size() - pos) {
		len_expected = sv_f.size() - pos;
		p = z_copy;
	}
	is_true(rlen == len_expected, "sv_f.copy(z_copy, n, pos).length");
	is_zero(strcmp(z_copy, p), "z_copy == z_copy_expected");

	try {
		//throw std::out_of_range exception if pos > size() 
		sv_f.copy(z_copy, n, sv_f.size() + 1);
		is_true(false, "sv_f.copy(z_copy, n, pos), should not be executed.");
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
	len_expected = n;

	string_view sub_sv{ sv.substr(pos, n) };
	//substr(size_type pos = 0, size_type count = npos)
	//Returns a view of the substring [pos, pos + rcount), 
	//where rcount is the smaller of count and size() - pos
	if (n > sv.size() - pos) {
		len_expected = sv.size() - pos;
	}
	is_true(sub_sv.size() == len_expected, "sv.copy(pos, n).size");
	is_true(sub_sv.data() == sv.data() + pos, "sv.copy(pos, n).data");
	is_true(sub_sv == sub_expected, "sv.copy(pos, n)");
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
	is_negative(sv_empty.compare(sv_a_to_e), "sv_empty.compare(sv_a_to_e)");
	is_positive(sv_a_to_c.compare(sv_empty), "sv_a_to_c.compare(sv_empty)");

	const char* a_ABC{ "ABC" };
	is_positive(sv_a_to_e.compare(a_ABC), "sv_a_to_e.compare(a_ABC)");
	is_zero(sv_A_to_E.compare(0, 3, a_ABC), "sv_A_to_E.compare(0, 3, a_ABC)");
	is_positive(sv_a_to_c.compare(a_ABC), "sv_a_to_c.compare(a_ABC)");


	//8. Searching
	//                   0         1 
	//                   01234567890123456
	string_view sv_ats{ "access to success" };
	is_true(sv_ats.find("ce") == 2, "sv_ats.find(str)");
	is_true(sv_ats.rfind("ce") == 13, "sv_ats.rfind(str)");
	is_zero(sv_ats.find(""), "sv_ats.find(str)");
	is_true(sv_ats.rfind("") == sv_ats.size(), "sv_ats.rfind(str)");
	is_true(sv_ats.find("TO") == string_view::npos, "sv_ats.find(str)");
	is_true(sv_ats.rfind("as") == string_view::npos, "sv_ats.rfind(str)");

	is_true(sv_ats.find_first_of("cess") == 1, "sv_ats.find_first_of(str)");
	is_true(sv_ats.find_last_of("cce") == 14, "sv_ats.find_last_of(str)");
	is_true(sv_ats.find_first_of("") == string_view::npos, "sv_ats.find_first_of(str)");
	is_true(sv_ats.find_last_of("") == string_view::npos, "sv_ats.find_last_of(str)");
	is_true(sv_ats.find_first_of("sT") == 4, "sv_ats.find_first_of(str)");
	is_true(sv_ats.find_last_of("wc") == 13, "sv_ats.find_last_of(str)");

	is_true(sv_ats.find_first_not_of("ac") == 3, "sv_ats.find_first_not_of(str)");
	is_true(sv_ats.find_last_not_of("cess") == 11, "sv_ats.find_last_not_of(str)");
	is_zero(sv_ats.find_first_not_of("success"), "sv_ats.find_first_not_of(str)");
	is_true(sv_ats.find_last_not_of("success") == 9, "sv_ats.find_last_not_of(str)");
	is_zero(sv_ats.find_first_not_of(""), "sv_ats.find_first_not_of(str)");
	is_true(sv_ats.find_last_not_of("") == 16, "sv_ats.find_last_not_of(str)");
}
