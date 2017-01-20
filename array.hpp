#pragma once

namespace matsulib
{
	template <class _T> class Array;
}

#include <vector>
#include <functional>

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

	auto each_with_index(std::function <void(const _T &value, size_type index)> func) const -> void
	{
		const auto length = this->size();
		for (size_type i = 0; i < length; ++i)
		{
			func(this->at(i), i);
		}
	}
	auto each(std::function <void(const _T &value)> func) const
	{
		return each_with_index(std::bind(func, std::placeholders::_1));
	}

	auto transform_with_index(std::function <_T(_T value, size_type index)> func) -> void
	{
		const auto length = this->size();
		for (size_type i = 0; i < length; ++i)
		{
			this->at(i) = func(std::move(this->at(i)), i);
		}
	}
	auto transform(std::function <_T(_T value)> func)
	{
		return transform_with_index(std::bind(func, std::placeholders::_1));
	}

	auto select_with_index(std::function <bool(const _T &value, size_type index)> func) const -> Array <_T>
	{
		const auto length = this->size();
		auto dst_array = Array <_T>{};
		dst_array.reserve(length);
		for (size_type i = 0; i < length; ++i)
		{
			if (func(this->at(i), i))
			{
				dst_array.push_back(this->at(i));
			}
		}
		return dst_array;
	}
	auto select(std::function <bool(const _T &value)> func) const
	{
		return select_with_index([&](const _T &value, size_type index) {return func(value); });
	}

	template <class _DistType>
	auto map_with_index(std::function <_DistType(const _T &value, size_type index)> func) const -> Array <_DistType>
	{
		const auto length = this->size();
		auto dst_array = Array <_DistType>{};
		dst_array.resize(length);
		for (size_type i = 0; i < length; ++i)
		{
			dst_array.at(i) = func(this->at(i), i);
		}
		return dst_array;
	}
	auto map_with_index(std::function <_T(const _T &value, size_type index)> func) const
	{
		return map_with_index <_T>(func);
	}
	template <class _DistType>
	auto map(std::function <_DistType(const _T &value)> func) const
	{
		return map_with_index <_DistType>(std::bind(func, std::placeholders::_1));
	}
	auto map(std::function <_T(const _T &value)> func) const
	{
		return map_with_index(std::bind(func, std::placeholders::_1));
	}

	template <class _DistType>
	_DistType inject_with_index(_DistType initial_value, std::function <_DistType(_DistType accumulation, const _T &value, size_type index)> func) const
	{
		auto accumulation = std::move(initial_value);
		const auto length = this->size();
		for (size_type i = 0; i < length; ++i)
		{
			accumulation = func(std::move(accumulation), this->at(i), i);
		}
		return accumulation;
	}
	_T inject_with_index(_T initial_value, std::function <_T(_T accumulation, const _T &value, size_type index)> func) const
	{
		return inject_with_index <_T>(std::move(initial_value), func);
	}
	template <class _DistType>
	_DistType inject(_DistType initial_value, std::function <_DistType(_DistType accumulation, const _T &value)> func) const
	{
		return inject_with_index <_DistType>(std::move(initial_value), std::bind(func, std::placeholders::_1, std::placeholders::_2));
	}
	_T inject(_T initial_value, std::function <_T(_T accumulation, const _T &value)> func) const
	{
		return inject_with_index(std::move(initial_value), std::bind(func, std::placeholders::_1, std::placeholders::_2));
	}
	template <class _DistType>
	_DistType inject(std::function <_DistType(_DistType accumulation, const _T &value)> func) const
	{
		return inject <_DistType>({}, func);
	}
	_T inject(std::function <_T(_T accumulation, const _T &value)> func) const
	{
		return inject({}, func);
	}
};