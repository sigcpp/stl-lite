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
#include <algorithm>
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

		//unchecked iterators: not standards-compliant; implementing for demo
		using iterator = T*; 
		using const_iterator = const T*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		//underlying array
		value_type elements[N];

		//utility
		void fill(const T& u) { std::fill_n(elements, N, u); }

		void swap(array& a) noexcept(std::is_nothrow_swappable_v<T>)
		{
			std::swap_ranges(elements, elements + N, a.elements);
		}

		//iterators
		constexpr iterator begin() noexcept 
		{ 
			if constexpr (N == 0)
				return nullptr;
			else
				return elements; 
		}
		
		constexpr const_iterator begin() const noexcept 
		{ 
			if constexpr (N == 0)
				return nullptr;
			else
				return elements;
		}

		constexpr iterator end() noexcept
		{
			if constexpr (N == 0)
				return nullptr;
			else
				return elements + N;
		}

		constexpr const_iterator end() const noexcept
		{
			if constexpr (N == 0)
				return nullptr;
			else
				return elements + N;
		}

		constexpr reverse_iterator rbegin() noexcept 
		{ 
			return reverse_iterator(end()); 
		}
		
		constexpr const_reverse_iterator rbegin() const noexcept 
		{ 
			return const_reverse_iterator(end()); 
		}
		
		constexpr reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
		}

		constexpr const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		constexpr const_iterator cbegin() const noexcept { return begin(); }
		constexpr const_iterator cend() const noexcept { return end(); }
		
		constexpr const_reverse_iterator crbegin() const noexcept 
		{ 
			return rbegin(); 
		}
		
		constexpr const_reverse_iterator crend() const noexcept { return rend(); }

		//capacity
		constexpr bool empty() const noexcept { return N == 0; }
		constexpr size_type size() const noexcept { return N; }
		constexpr size_type max_size() const noexcept { return N; }

		//unchecked element access
		constexpr reference operator[](size_type pos) { return elements[pos]; }
		
		constexpr const_reference operator[](size_type pos) const 
		{ 
			return elements[pos]; 
		}

		//checked element access
		constexpr reference at(size_type pos)
		{
			if (pos < N)
				return elements[pos];
			else
				throw new std::out_of_range("array index out of range");
		}

		constexpr const_reference at(size_type pos) const
		{
			if (pos < N)
				return elements[pos];
			else
				throw new std::out_of_range("array index out of range");
		}

		constexpr reference front() { return elements[0]; }
		constexpr const_reference front() const { return elements[0]; }

		constexpr reference back() { return empty() ? front() : elements[N-1]; }
		
		constexpr const_reference back() const 
		{ 
			if constexpr (N == 0)
				return front();
			else
				return elements[N - 1];
		}

		//underlying raw data
		constexpr pointer data() noexcept 
		{
			if constexpr (N == 0)
				return nullptr;
			else
				return elements;
		}

		constexpr const_pointer data() const noexcept 
		{ 
			if constexpr (N == 0)
				return nullptr;
			else
				return elements;
		}

	}; //template array

}	//namespace sigcpp

#endif