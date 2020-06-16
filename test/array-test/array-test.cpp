/*
* array-test.cpp
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Test array template
* - see C++17 [array.overview] https://timsong-cpp.github.io/cppwp/n4659/array
*/

#include <algorithm>

#include "../../include/array.h"

#include "../verifiers.h"

void array_test()
{
	using sigcpp::array;

	//non-empty array with full init
	array<short, 3> s{ 8, -2, 7 };

	//non-empty array with partial init: last two elements are 0
	array<short, 5> p{ 8, -2, 7 };


	//capacity
	is_false(s.empty(), "s.empty()");
	is_true(s.size() == 3, "s.size()");
	is_true(s.max_size() == s.size(), "s.max_size()");

	is_false(p.empty(), "p.empty()");
	is_true(p.size() == 5, "p.size()");
	is_true(p.max_size() == p.size(), "p.max_size()");


	//element access
	is_true(s[0] == 8, "s[0]");
	is_true(s[1] == -2, "s[1]");
	is_true(s[2] == 7, "s[2]");
	is_true(s[1] != 8, "s[1] != 8");

	is_true(p[0] == 8, "p[0]");
	is_true(p[2] == 7, "p[2]");
	is_true(p[4] == 0, "p[4]");

	is_true(s.at(0) == 8, "s.at(0)");
	is_true(s.at(1) == -2, "s.at(1)");
	is_true(s.at(2) == 7, "s.at(2)");

	is_true(p.at(0) == 8, "p.at(0)");
	is_true(p.at(2) == 7, "p.at(2)");
	is_true(p.at(4) == 0, "p.at(4)");

	is_true(s.front() == 8, "s.front()");
	is_true(s.front() != -2, "s.front() != -2");
	is_true(s.back() == 7, "s.back()");
	is_true(s.back() != -2, "s.back() != -2");

	is_true(p.front() == 8, "p.front()");
	is_true(p.front() != -2, "p.front() != -2");
	is_true(p.back() == 0, "p.back()");


	//forward iterators
	array<unsigned, 5> u{ 5, 9, 3, 1, 6 };
	unsigned uExpected[] = { 5, 9, 3, 1, 6 };

	bool iteratorTest = true;
	std::size_t i = 0;
	for (auto it = u.begin(); it != u.end() && iteratorTest; ++it, ++i)
		iteratorTest = *it == uExpected[i];
	is_true(iteratorTest, "forward iterator");

	//reverse iterators
	unsigned urExpected[] = { 6, 1, 3, 9, 5 };

	iteratorTest = true;
	i = 0;
	for (auto it = u.rbegin(); it != u.rend() && iteratorTest; ++it, ++i)
		iteratorTest = *it == urExpected[i];
	is_true(iteratorTest, "reverse iterator");

	//zero-size array
	array<char, 0> c;
	is_true(c.empty(), "c.empty()");

	//iterator on empty array: the loop body should not execute
	iteratorTest = true;
	for (const auto e : c)
		iteratorTest = false;
	is_true(iteratorTest, "fwd iterator on empty array");


	//fill
	array<char, 5> a;
	a.fill('x');

	//test until one of the elements is *not* the fill value
	//-false from any_of call means fill succeeded
	bool fillTest = !std::any_of(a.begin(), a.end(),
		[](char c) { return c != 'x'; }
	);
	is_true(fillTest, "a.fill()");


	//swap
	array<int, 3> m{ 8, 5, 4 };
	array<int, 3> n{ 10, -2, 7 };
	int mExpected[] = { 10, -2, 7 };
	int nExpected[] = { 8, 5, 4 };

	m.swap(n);

	bool swapTest = true;
	for (std::size_t idx = 0; idx < m.size() && swapTest; ++idx)
		swapTest = m[idx] == mExpected[idx] && n[idx] == nExpected[idx];
	is_true(swapTest, "m.swap(n)");
}
