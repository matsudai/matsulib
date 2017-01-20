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

	auto each_with_index(std::function <void(const _T &value, size_type index)> func) const -> void;
	auto each(std::function <void(const _T &value)> func) const -> void;

	auto transform_with_index(std::function <_T(_T value, size_type index)> func) -> Array <_T> &;
	auto transform(std::function <_T(_T value)> func)->Array <_T> &;

	auto select_with_index(std::function <bool(const _T &value, size_type index)> func) const -> Array <_T>;
	auto select(std::function <bool(const _T &value)> func) const -> Array <_T>;

	template <class _DistType>
	auto map_with_index(std::function <_DistType(const _T &value, size_type index)> func) const -> Array <_DistType>;
	auto map_with_index(std::function <_T(const _T &value, size_type index)> func) const -> Array <_T>;
	template <class _DistType>
	auto map(std::function <_DistType(const _T &value)> func) const -> Array <_DistType>;
	auto map(std::function <_T(const _T &value)> func) const -> Array <_T>;
	
	template <class _DistType>
	auto inject_with_index(_DistType initial_value, std::function <_DistType(_DistType accumulation, const _T &value, size_type index)> func) const -> _DistType;
	auto inject_with_index(_T initial_value, std::function <_T(_T accumulation, const _T &value, size_type index)> func) const -> _T;
	template <class _DistType>
	auto inject(_DistType initial_value, std::function <_DistType(_DistType accumulation, const _T &value)> func) const ->_DistType;
	auto inject(_T initial_value, std::function <_T(_T accumulation, const _T &value)> func) const -> _T;
	template <class _DistType>
	auto inject(std::function <_DistType(_DistType accumulation, const _T &value)> func) const -> _DistType;
	auto inject(std::function <_T(_T accumulation, const _T &value)> func) const -> _T;
};

template <class _T>
inline
auto matsulib::Array <_T>::each_with_index(std::function <void(const _T &value, size_type index)> func) const -> void
{
	const auto length = this->size();
	for (size_type i = 0; i < length; ++i)
	{
		func(this->at(i), i);
	}
}

template <class _T>
inline
auto matsulib::Array <_T>::each(std::function <void(const _T &value)> func) const -> void
{
	return each_with_index(std::bind(func, std::placeholders::_1));
}

template <class _T>
inline
auto matsulib::Array <_T>::transform_with_index(std::function <_T(_T value, size_type index)> func) -> Array <_T> &
{
	const auto length = this->size();
	for (size_type i = 0; i < length; ++i)
	{
		this->at(i) = func(std::move(this->at(i)), i);
	}
	return *this;
}

template <class _T>
inline
auto matsulib::Array <_T>::transform(std::function <_T(_T value)> func) -> Array <_T> &
{
	return transform_with_index(std::bind(func, std::placeholders::_1));
}

template <class _T>
inline
auto matsulib::Array <_T>::select_with_index(std::function <bool(const _T &value, size_type index)> func) const -> Array <_T>
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
template <class _T>
inline
auto matsulib::Array <_T>::select(std::function <bool(const _T &value)> func) const -> Array <_T>
{
	return select_with_index(std::bind(func, std::placeholders::_1));
}

template <class _T>
template <class _DistType>
inline
auto matsulib::Array <_T>::map_with_index(std::function <_DistType(const _T &value, size_type index)> func) const -> Array <_DistType>
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

template <class _T>
inline
auto matsulib::Array <_T>::map_with_index(std::function <_T(const _T &value, size_type index)> func) const -> Array <_T>
{
	return map_with_index <_T>(func);
}

template <class _T>
template <class _DistType>
inline
auto matsulib::Array <_T>::map(std::function <_DistType(const _T &value)> func) const -> Array <_DistType>
{
	return map_with_index <_DistType>(std::bind(func, std::placeholders::_1));
}

template <class _T>
inline
auto matsulib::Array <_T>::map(std::function <_T(const _T &value)> func) const -> Array <_T>
{
	return map_with_index(std::bind(func, std::placeholders::_1));
}

template <class _T>
template <class _DistType>
inline
auto matsulib::Array <_T>::inject_with_index(_DistType initial_value, std::function <_DistType(_DistType accumulation, const _T &value, size_type index)> func) const -> _DistType
{
	auto accumulation = std::move(initial_value);
	const auto length = this->size();
	for (size_type i = 0; i < length; ++i)
	{
		accumulation = func(std::move(accumulation), this->at(i), i);
	}
	return accumulation;
}

template <class _T>
inline
auto matsulib::Array <_T>::inject_with_index(_T initial_value, std::function <_T(_T accumulation, const _T &value, size_type index)> func) const -> _T
{
	return inject_with_index <_T>(std::move(initial_value), func);
}

template <class _T>
template <class _DistType>
inline
auto matsulib::Array <_T>::inject(_DistType initial_value, std::function <_DistType(_DistType accumulation, const _T &value)> func) const -> _DistType
{
	return inject_with_index <_DistType>(std::move(initial_value), std::bind(func, std::placeholders::_1, std::placeholders::_2));
}

template <class _T>
inline
auto matsulib::Array <_T>::inject(_T initial_value, std::function <_T(_T accumulation, const _T &value)> func) const -> _T
{
	return inject_with_index(std::move(initial_value), std::bind(func, std::placeholders::_1, std::placeholders::_2));
}

template <class _T>
template <class _DistType>
inline
auto matsulib::Array <_T>::inject(std::function <_DistType(_DistType accumulation, const _T &value)> func) const -> _DistType
{
	return inject <_DistType>({}, func);
}

template <class _T>
inline
auto matsulib::Array <_T>::inject(std::function <_T(_T accumulation, const _T &value)> func) const -> _T
{
	return inject({}, func);
}