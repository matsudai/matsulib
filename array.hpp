#pragma once

namespace matsulib
{
	template <class _T> class Array;
}

#include <vector>

template <class _T>
class matsulib::Array
	: public std::vector <_T>
{
public:
	Array() = default;
	Array(const Array &) = default;
	Array(Array &&) = default;
	Array &operator =(const Array &) = default;
	Array &operator =(Array &&) = default;
	virtual ~Array() = default;

	using parent = std::vector <_T>;
	using parent::parent;
};