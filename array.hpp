#pragma once

namespace matsulib
{
  template <class T> class Array;
}

#include <vector>
#include <functional>

template <class T>
class matsulib::Array
  : public std::vector <T>
{
public:
  Array() = default;
  Array(const Array &) = default;
  Array(Array &&) = default;
  Array &operator =(const Array &) = default;
  Array &operator =(Array &&) = default;
  virtual ~Array() = default;

  using parent = std::vector <T>;
  using parent::parent;

  auto each_with_index(std::function <void(const T &value, size_type index)> func) const -> const Array <T> &;
  auto each_with_index(std::function <void(const T &value, size_type index)> func) -> Array <T> &;
  auto each(std::function <void(const T &value)> func) const -> const Array <T> &;
  auto each(std::function <void(const T &value)> func) -> Array <T> &;

  auto transform_with_index(std::function <T(T value, size_type index)> func) -> Array <T> &;
  auto transform(std::function <T(T value)> func)->Array <T> &;

  auto select_with_index(std::function <bool(const T &value, size_type index)> func) const -> Array <T>;
  auto select(std::function <bool(const T &value)> func) const -> Array <T>;

  template <class DistType>
  auto map_with_index(std::function <DistType(const T &value, size_type index)> func) const -> Array <DistType>;
  auto map_with_index(std::function <T(const T &value, size_type index)> func) const -> Array <T>;
  template <class DistType>
  auto map(std::function <DistType(const T &value)> func) const -> Array <DistType>;
  auto map(std::function <T(const T &value)> func) const -> Array <T>;

  template <class DistType>
  auto inject_with_index(DistType initial_value, std::function <DistType(DistType accumulation, const T &value, size_type index)> func) const -> DistType;
  auto inject_with_index(T initial_value, std::function <T(T accumulation, const T &value, size_type index)> func) const -> T;
  template <class DistType>
  auto inject(DistType initial_value, std::function <DistType(DistType accumulation, const T &value)> func) const ->DistType;
  auto inject(T initial_value, std::function <T(T accumulation, const T &value)> func) const -> T;
  template <class DistType>
  auto inject(std::function <DistType(DistType accumulation, const T &value)> func) const -> DistType;
  auto inject(std::function <T(T accumulation, const T &value)> func) const -> T;
};

template <class T>
inline
auto matsulib::Array <T>::each_with_index(std::function <void(const T &value, size_type index)> func) const -> const Array <T> &
{
  const auto length = this->size();
  for (size_type i = 0; i < length; ++i)
  {
    func(this->at(i), i);
  }
  return *this;
}

template <class T>
inline
auto matsulib::Array <T>::each_with_index(std::function <void(const T &value, size_type index)> func) -> Array <T> &
{
  return const_cast <Array <T> &>(static_cast <const Array <T> &>(*this).each_with_index(func));
}

template <class T>
inline
auto matsulib::Array <T>::each(std::function <void(const T &value)> func) const -> const Array <T> &
{
  return each_with_index(std::bind(func, std::placeholders::_1));
}

template <class T>
inline
auto matsulib::Array <T>::each(std::function <void(const T &value)> func) -> Array <T> &
{
  return each_with_index(std::bind(func, std::placeholders::_1));
}


template <class T>
inline
auto matsulib::Array <T>::transform_with_index(std::function <T(T value, size_type index)> func) -> Array <T> &
{
  const auto length = this->size();
  for (size_type i = 0; i < length; ++i)
  {
    this->at(i) = func(std::move(this->at(i)), i);
  }
  return *this;
}

template <class T>
inline
auto matsulib::Array <T>::transform(std::function <T(T value)> func) -> Array <T> &
{
  return transform_with_index(std::bind(func, std::placeholders::_1));
}

template <class T>
inline
auto matsulib::Array <T>::select_with_index(std::function <bool(const T &value, size_type index)> func) const -> Array <T>
{
  const auto length = this->size();
  auto dst_array = Array <T>{};
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
template <class T>
inline
auto matsulib::Array <T>::select(std::function <bool(const T &value)> func) const -> Array <T>
{
  return select_with_index(std::bind(func, std::placeholders::_1));
}

template <class T>
template <class DistType>
inline
auto matsulib::Array <T>::map_with_index(std::function <DistType(const T &value, size_type index)> func) const -> Array <DistType>
{
  const auto length = this->size();
  auto dst_array = Array <DistType>{};
  dst_array.resize(length);
  for (size_type i = 0; i < length; ++i)
  {
    dst_array.at(i) = func(this->at(i), i);
  }
  return dst_array;
}

template <class T>
inline
auto matsulib::Array <T>::map_with_index(std::function <T(const T &value, size_type index)> func) const -> Array <T>
{
  return map_with_index <T>(func);
}

template <class T>
template <class DistType>
inline
auto matsulib::Array <T>::map(std::function <DistType(const T &value)> func) const -> Array <DistType>
{
  return map_with_index <DistType>(std::bind(func, std::placeholders::_1));
}

template <class T>
inline
auto matsulib::Array <T>::map(std::function <T(const T &value)> func) const -> Array <T>
{
  return map_with_index(std::bind(func, std::placeholders::_1));
}

template <class T>
template <class DistType>
inline
auto matsulib::Array <T>::inject_with_index(DistType initial_value, std::function <DistType(DistType accumulation, const T &value, size_type index)> func) const -> DistType
{
  auto accumulation = std::move(initial_value);
  const auto length = this->size();
  for (size_type i = 0; i < length; ++i)
  {
    accumulation = func(std::move(accumulation), this->at(i), i);
  }
  return accumulation;
}

template <class T>
inline
auto matsulib::Array <T>::inject_with_index(T initial_value, std::function <T(T accumulation, const T &value, size_type index)> func) const -> T
{
  return inject_with_index <T>(std::move(initial_value), func);
}

template <class T>
template <class DistType>
inline
auto matsulib::Array <T>::inject(DistType initial_value, std::function <DistType(DistType accumulation, const T &value)> func) const -> DistType
{
  return inject_with_index <DistType>(std::move(initial_value), std::bind(func, std::placeholders::_1, std::placeholders::_2));
}

template <class T>
inline
auto matsulib::Array <T>::inject(T initial_value, std::function <T(T accumulation, const T &value)> func) const -> T
{
  return inject_with_index(std::move(initial_value), std::bind(func, std::placeholders::_1, std::placeholders::_2));
}

template <class T>
template <class DistType>
inline
auto matsulib::Array <T>::inject(std::function <DistType(DistType accumulation, const T &value)> func) const -> DistType
{
  return inject <DistType>({}, func);
}

template <class T>
inline
auto matsulib::Array <T>::inject(std::function <T(T accumulation, const T &value)> func) const -> T
{
  return inject({}, func);
}