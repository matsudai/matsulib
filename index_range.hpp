#pragma once

#include "has_iterator.hpp"
#include <type_traits>

namespace matsulib
{
  template <class> class IndexRange;

  namespace _detail
  {
    namespace index_range
    {
      template <class _has_itr_flag> struct Range;
      template <> struct Range <std::true_type>
      {
        template <class _Container>
        static auto calc(const _Container &obj) -> decltype(obj.end() - obj.begin()) { return obj.end() - obj.begin(); }
      };
      template <> struct Range <std::false_type>
      {
        template <class Index>
        static auto calc(Index range) -> Index { return range; }
      };
    }
  }

  template <class T>
  auto index_range(const T &begin, const T &end)
  {
    static_assert(!has_iterator<T>::value, "index_range() : Args must not be Container !!");
    return IndexRange <T>{begin, end};
  }
  template <class T>
  auto index_range(const T &range)
  {
    auto range_value = _detail::index_range::Range <typename has_iterator <T>::type>::calc(range);
    return IndexRange <decltype(range_value)>{range_value};
  }
}

template <class Index>
class matsulib::IndexRange
{
protected:
  Index _end;
  Index _index;

public:
  // range = [begin, ..., end)
  IndexRange(Index begin, Index end) : _end{ end }, _index{ begin } {}
  // range = [0, ..., range)
  explicit IndexRange(Index range) : _end{ range }, _index { 0 } {}

public:
  IndexRange() = delete;
  IndexRange(const IndexRange &) = default;
  IndexRange(IndexRange &&) = default;
  IndexRange &operator =(const IndexRange &) = default;
  IndexRange &operator =(IndexRange &&) = default;

public:
  auto operator *() -> Index { return _index; }
  auto operator !=(IndexRange &) -> bool { return _index < _end; }
  auto operator ++() -> void { ++_index; }

  auto begin() const -> IndexRange { return *this; }
  auto end() const -> IndexRange { return *this; }
};