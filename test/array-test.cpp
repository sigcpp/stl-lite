/*
* array-test.cpp
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice shown on lines 3 and 4 must be retained.
* That information may be relocated but be conspicuous in all derived work.
*
* Test array template
* - see C++17 [array.overview] https://timsong-cpp.github.io/cppwp/n4659/array
*/

#include <iostream>
#include <exception>
#include <algorithm>
#include <string>

#include "../include/array.h"

#include "tester.h"

using std::cout;


void runTests();

int main()
{
   try
   {
      cout << "\nRunning tests: ";
      runTests();
      summarizeTests();
   }
   catch (const std::string& msg)
   {
      cout << '\n' << msg << '\n';
   }
}

void runTests()
{
   using sigcpp::array;


   //non-empty array with full init
   array<short, 3> s{ 8, -2, 7 };

   //non-empty array with partial init: last two elements are 0
   array<short, 5> p{ 8, -2, 7};


   //capacity
   assert(!s.empty(), "s.empty()");
   assert(s.size() == 3, "s.size()");
   assert(s.max_size() == s.size(), "s.max_size()");

   assert(!p.empty(), "p.empty()");
   assert(p.size() == 5, "p.size()");
   assert(p.max_size() == p.size(), "p.max_size()");


   //element access
   assert(s[0] == 8, "s[0]");
   assert(s[1] == -2, "s[1]");
   assert(s[2] == 7, "s[2]");
   assert(s[1] != 8, "s[1] != 8");

   assert(p[0] == 8, "p[0]");
   assert(p[2] == 7, "p[2]");
   assert(p[4] == 0, "p[4]");

   assert(s.at(0) == 8, "s.at(0)");
   assert(s.at(1) == -2, "s.at(1)");
   assert(s.at(2) == 7, "s.at(2)");

   assert(p.at(0) == 8, "p.at(0)");
   assert(p.at(2) == 7, "p.at(2)");
   assert(p.at(4) == 0, "p.at(4)");

   assert(s.front() == 8, "s.front()");
   assert(s.front() != -2, "s.front() != -2");
   assert(s.back() == 7, "s.back()");
   assert(s.back() != -2, "s.back() != -2");

   assert(p.front() == 8, "p.front()");
   assert(p.front() != -2, "p.front() != -2");
   assert(p.back() == 0, "p.back()");


   //forward iterators
   array<unsigned, 5> u{ 5, 9, 3, 1, 6 };
   unsigned uExpected[] = { 5, 9, 3, 1, 6 };

   bool iteratorTest = true;
   std::size_t i = 0;
   for (auto it = u.begin(); it != u.end() && iteratorTest; it++, i++)
      iteratorTest = *it == uExpected[i];
   assert(iteratorTest, "forward iterator");

   //reverse iterators
   unsigned urExpected[] = { 6, 1, 3, 9, 5 };

   iteratorTest = true;
   i = 0;
   for (auto it = u.rbegin(); it != u.rend() && iteratorTest; it++, i++)
      iteratorTest = *it == urExpected[i];
   assert(iteratorTest, "reverse iterator");

   //zero-size array
   array<char, 0> c;
   assert(c.empty(), "c.empty()");

   //iterator on empty array: the loop body should not execute
   iteratorTest = true;
   for (const auto e : c)
      iteratorTest = false;
   assert(iteratorTest, "fwd iterator on empty array");


   //fill
   array<char, 5> a;
   a.fill('x');

   //test until one of the elements is *not* the fill value
   //-false from any_of call means fill succeeded
   bool fillTest = !std::any_of(a.begin(), a.end(), 
                                [](char c) { return c != 'x'; }
                               );
   assert(fillTest, "a.fill()");


   //swap
   array<int, 3> m{ 8, 5, 4 };
   array<int, 3> n{ 10, -2, 7 };
   int mExpected[] = { 10, -2, 7 };
   int nExpected[] = { 8, 5, 4 };

   m.swap(n);

   bool swapTest = true;
   for (std::size_t idx = 0; idx < m.size() && swapTest; idx++)
      swapTest = m[idx] == mExpected[idx] && n[idx] == nExpected[idx];
   assert(swapTest, "m.swap(n)");
}


//print content of any collection
template<typename ForwardIt>
void print(ForwardIt first, ForwardIt last, const char* heading = nullptr)
{
   if (heading)
      cout << heading << ": ";

   for (; first != last; ++first)
      cout << *first << ' ';

   cout << '\n';
}