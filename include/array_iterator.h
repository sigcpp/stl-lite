/*
* array_iterator.h
* Sean Murthy
* (c) 2020 sigcpp https://sigcpp.github.io. See LICENSE.MD
*
* Attribution and copyright notice shown on lines 3 and 4 must be retained.
* That info may be relocated but it must be conspicuous in all derived work.
*
* Define a template for random-access iterators: wraps a raw array
* - see C++17 [iterator.requirements]
* - https://timsong-cpp.github.io/cppwp/n4659/iterator.requirements
*/

#ifndef SIGCPP_ARRAY_ITERATOR_H
#define SIGCPP_ARRAY_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace sigcpp
{
	template<typename P> //P is a pointer type
	class array_iterator
	{
	public:

		//types
		using iterator_category = typename std::iterator_traits<P>::iterator_category;
		using value_type = typename std::iterator_traits<P>::value_type;
		using difference_type = typename std::iterator_traits<P>::difference_type;
		using pointer = typename std::iterator_traits<P>::pointer;
		using reference = typename std::iterator_traits<P>::reference;

		//ctors
		array_iterator() noexcept = default;
		array_iterator(P p) noexcept : basePtr(p){}

		//the wrapped iter
		constexpr P base() const noexcept { return basePtr; }

		//dereference and member access
		constexpr reference operator*() const { return *basePtr; }
		constexpr pointer operator->() const noexcept { return basePtr; }
		
		//element access
		constexpr reference operator[](difference_type n) const
		{
			return basePtr[n];
		}

		//increment and decrement
		constexpr array_iterator& operator++() 
		{
			++basePtr;
			return *this;
		}

		constexpr array_iterator operator++(int) 
		{
			array_iterator beforeIncrement = *this;
			++basePtr;
			return beforeIncrement;
		}

		constexpr array_iterator& operator--() 
		{
			--basePtr;
			return *this;
		}

		constexpr array_iterator operator--(int) 
		{
			array_iterator beforeDecrement = *this;
			--basePtr;
			return beforeDecrement;
		}

		//arithmetic
		constexpr array_iterator operator+(difference_type n) const
		{
			array_iterator t = *this;
			t += n;
			return t;
		}

		constexpr array_iterator operator-(difference_type n) const
		{
			array_iterator t = *this;
			t -= n;
			return t;
		}

		array_iterator& operator+=(difference_type n)
		{
			basePtr += n;
			return *this;
		}

		array_iterator& operator-=(difference_type n)
		{
			basePtr -= n;
			return *this;
		}

		//comparison
		constexpr bool operator==(const array_iterator& r) const 
		{
			return basePtr == r.basePtr;
		}

		constexpr bool operator!=(const array_iterator& r) const
		{
			return basePtr != r.basePtr;
		}

		constexpr bool operator<(const array_iterator& r) const
		{
			return basePtr < r.basePtr;
		}

		constexpr bool operator>(const array_iterator& r) const
		{
			return basePtr > r.basePtr;
		}

		constexpr bool operator<=(const array_iterator& r) const
		{
			return !(r > *this);
		}

		constexpr bool operator>=(const array_iterator& r) const
		{
			return !(*this < r);
		}

	private:
		P basePtr{ nullptr };

	}; //template array_iterator

}	//namespace sigcpp

#endif