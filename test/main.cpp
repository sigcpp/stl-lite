/*
* main.cpp
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io
*
* Test stl-lite
*/


#include <iostream>

#include "../include/array.h"

int main()
{
    using sigcpp::array;
    
    array<int, 10> i;
    array<float, 5> f;
    array<double, 25> d;

    array<int, 10>::size_type m = i.size();

    const array<float, 5>::size_type n = f.size();

    constexpr array<double, 25>::size_type p = d.size();

    std::cout << m << ' ' << n << ' ' << p << '\n';
}