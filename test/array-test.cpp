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

#include "../include/array.h"

using std::cout;

void test(bool success, const char* msg);
void summarizeTests();

int main()
{
   using sigcpp::array;

   cout << "\nRunning tests ";

   //non-empty array with full init
   array<short, 3> s{ 8, -2, 7 };

   //non-empty array with partial init: last two elements are 0
   array<short, 5> p{ 8, -2, 7};


   //capacity
   test(!s.empty(), "s.empty()");
   test(s.size() == 3, "s.size()");
   test(s.max_size() == s.size(), "s.max_size()");

   test(!p.empty(), "p.empty()");
   test(p.size() == 5, "p.size()");
   test(p.max_size() == p.size(), "p.max_size()");


   //element access
   test(s[0] == 8, "s[0]");
   test(s[1] == -2, "s[1]");
   test(s[2] == 7, "s[2]");
   test(s[1] != 8, "s[1] != 8");

   test(p[0] == 8, "p[0]");
   test(p[2] == 7, "p[2]");
   test(p[4] == 0, "p[4]");

   test(s.at(0) == 8, "s.at(0)");
   test(s.at(1) == -2, "s.at(1)");
   test(s.at(2) == 7, "s.at(2)");

   test(p.at(0) == 8, "p.at(0)");
   test(p.at(2) == 7, "p.at(2)");
   test(p.at(4) == 0, "p.at(4)");

   test(s.front() == 8, "s.front()");
   test(s.front() != -2, "s.front() != -2");
   test(s.back() == 7, "s.back()");
   test(s.back() != -2, "s.back() != -2");

   test(p.front() == 8, "p.front()");
   test(p.front() != -2, "p.front() != -2");
   test(p.back() == 0, "p.back()");


   //forward iterators
   array<unsigned, 5> u{ 5, 9, 3, 1, 6 };
   unsigned uExpected[] = { 5, 9, 3, 1, 6 };

   bool iteratorTest = true;
   std::size_t i = 0;
   for (auto it = u.begin(); it != u.end() && iteratorTest; it++, i++)
      iteratorTest = *it == uExpected[i];
   test(iteratorTest, "forward iterator");

   //reverse iterators
   unsigned urExpected[] = { 6, 1, 3, 9, 5 };

   iteratorTest = true;
   i = 0;
   for (auto it = u.rbegin(); it != u.rend() && iteratorTest; it++, i++)
      iteratorTest = *it == urExpected[i];
   test(iteratorTest, "reverse iterator");

   //zero-size array
   array<char, 0> c;
   test(c.empty(), "c.empty()");

   //iterator on empty array: the loop body should not execute
   iteratorTest = true;
   for (const auto e : c)
      iteratorTest = false;
   test(iteratorTest, "fwd iterator on empty array");


   //fill
   array<char, 5> a;
   a.fill('x');

   //test until one of the elements is *not* the fill value
   //-false from any_of call means fill succeeded
   bool fillTest = !std::any_of(a.begin(), a.end(), 
                                [](char c) { return c != 'x'; }
                               );
   test(fillTest, "a.fill()");


   //swap
   array<int, 3> m{ 8, 5, 4 };
   array<int, 3> n{ 10, -2, 7 };
   int mExpected[] = { 10, -2, 7 };
   int nExpected[] = { 8, 5, 4 };

   m.swap(n);

   bool swapTest = true;
   for (std::size_t idx = 0; idx < m.size() && swapTest; idx++)
      swapTest = m[idx] == mExpected[idx] && n[idx] == nExpected[idx];
   test(swapTest, "m.swap(n)");


   //summarize
   summarizeTests();
}


//counter for number of tests run
static unsigned testCount;

//track number of tests and check test result
void test(bool success, const char* msg)
{
   testCount++;

   if (success)
      cout << '.';
   else
   {
      cout << "\nFailed Test " << testCount << ": " << msg;
      throw std::exception();
   }
}


//print a simple test report
void summarizeTests()
{
   cout << "\nTests successful (" << testCount << " tests)\n";
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