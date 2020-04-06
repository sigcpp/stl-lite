/*
* array.h
* Sean Murthy 
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice must be retained.
* - Attribution may be augmented to include additional authors
* - Copyright notice cannot be altered
* Attribution and copyright info may be relocated but they must be conspicuous.
*
* Define a class template for arrays
* - see C++17 [array.overview], [array.syn]
* - https://timsong-cpp.github.io/cppwp/n4659/array
* - https://timsong-cpp.github.io/cppwp/n4659/array.syn
*/

#ifndef SIGCPP_ARRAY_H
#define SIGCPP_ARRAY_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <iterator>
#include <type_traits>

#include "array_iterator.h"

namespace sigcpp
{
	template<typename T, std::size_t N>
	struct array
	{
		//types
		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		//unchecked iterators: simple but standards-compliant
		using iterator = pointer; 
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		//underlying array
		value_type values[N];

		//utility
		void fill(const T& u) { std::fill_n(values, N, u); }

		void swap(array& a) noexcept(std::is_nothrow_swappable_v<T>)
		{
			std::swap_ranges(values, values + N, a.values);
		}

		//iterators
		constexpr iterator begin() noexcept { return _begin(); }
		constexpr const_iterator begin() const noexcept { return cbegin(); }
		constexpr iterator end() noexcept { return _end(); }
		constexpr const_iterator end() const noexcept { return cend(); }

		constexpr reverse_iterator rbegin() noexcept 
		{ 
			return reverse_iterator(_end());
		}
		
		constexpr const_reverse_iterator rbegin() const noexcept 
		{ 
			return crbegin();
		}
		
		constexpr reverse_iterator rend() noexcept
		{
			return reverse_iterator(_begin());
		}

		constexpr const_reverse_iterator rend() const noexcept
		{
			return crend();
		}

		constexpr const_iterator cbegin() const noexcept 
		{ 
			if constexpr (N == 0)
				return nullptr;
			else
				return values;
		}

		constexpr const_iterator cend() const noexcept
		{
			if constexpr (N == 0)
				return nullptr;
			else
				return values + N;
		}
		
		constexpr const_reverse_iterator crbegin() const noexcept 
		{ 
			return const_reverse_iterator(cend());
		}
		
		constexpr const_reverse_iterator crend() const noexcept 
		{ 
			return const_reverse_iterator(cbegin());
		}

		//capacity
		constexpr bool empty() const noexcept { return N == 0; }
		constexpr size_type size() const noexcept { return N; }
		constexpr size_type max_size() const noexcept { return N; }

		//unchecked element access
		constexpr reference operator[](size_type pos) { return values[pos]; }
		
		constexpr const_reference operator[](size_type pos) const 
		{ 
			return values[pos]; 
		}

		//checked element access
		constexpr reference at(size_type pos)
		{
			return const_cast<reference>(_at(pos));
		}

		constexpr const_reference at(size_type pos) const { return _at(pos); }

		constexpr reference front() { return values[0]; }
		constexpr const_reference front() const { return values[0]; }

		constexpr reference back() { return const_cast<reference>(_back()); }
		constexpr const_reference back() const { return _back(); }

		//underlying raw data
		constexpr pointer data() noexcept
		{
			return const_cast<pointer>(_data());
		}

		constexpr const_pointer data() const noexcept { return _data(); }

	private:
		//utility functions to eliminate redundancy in public members
		constexpr iterator _begin() noexcept
		{
			return const_cast<iterator>(cbegin());
		}

		constexpr iterator _end() noexcept
		{
			return const_cast<iterator>(cend());
		}

		constexpr const_reference _at(size_type pos) const
		{
			if (pos < N)
				return values[pos];
			else
				throw new std::out_of_range("array index out of range");
		}

		constexpr const_reference _back() const
		{
			if constexpr (N == 0)
				return front();
			else
				return values[N - 1];
		}

		constexpr const_pointer _data() const noexcept
		{
			if constexpr (N == 0)
				return nullptr;
			else
				return values;
		}

	}; //template array

}	//namespace sigcpp

#endif