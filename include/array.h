/*
* array.h
* Sean Murthy 
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice shown on lines 3 and 4 must be retained.
* That information may be relocated but be conspicuous in all derived work.
* 
* Define a class template for arrays
* - see C++17 [array.overview] https://timsong-cpp.github.io/cppwp/n4659/array
*/

#ifndef SIGCPP_ARRAY_H
#define SIGCPP_ARRAY_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <type_traits>

namespace sigcpp
{
	template<typename T, std::size_t N>
	struct array
	{
		//types
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		//using iterator = implementation-defined;
		//using const_iterator = implementation - defined;
		//using reverse_iterator = std::reverse_iterator<iterator>;
		//using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		//underlying array
		value_type elements[N];

		//utility
		void fill(const T& u) { for (auto& v : elements) v = u; }

		void swap(array& a) noexcept(std::is_nothrow_swappable_v<T>)
		{
			std::swap(elements, a.elements);
		}

		//capacity
		constexpr bool empty() const noexcept { return N == 0; }
		constexpr size_type size() const noexcept { return N; }
		constexpr size_type max_size() const noexcept { return N; }

		//unchecked element access
		constexpr reference operator[](size_type pos) { return elements[pos]; }
		constexpr const_reference operator[](size_type pos) const { return elements[pos]; }

		//checked element access
		constexpr reference at(size_type pos)
		{
			if (pos < N)
				return elements[pos];
			else
				throw new std::out_of_range("array index out of range");
		}

		constexpr const_reference at(size_type pos) const { return at(pos); }

		constexpr reference front() { return elements[0]; }
		constexpr const_reference front() const { return elements[0]; }

		constexpr reference back() { return elements[N-1]; }
		constexpr const_reference back() const { return elements[N-1]; }

		//underlying raw data
		constexpr pointer data() noexcept { return elements; }
		constexpr const_pointer data() const noexcept { return elements;  }

	}; //template array

}	//namespace sigcpp

#endif