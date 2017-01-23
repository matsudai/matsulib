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
        template <class _Index>
        static auto calc(_Index range) -> _Index { return range; }
      };
    }
  }

  template <class _T>
  auto index_range(const _T &begin, const _T &end)
  {
    static_assert(!has_iterator<_T>::value, "index_range() : Args must not be Container !!");
    return IndexRange <_T>{begin, end};
  }
  template <class _T>
  auto index_range(const _T &range)
  {
    auto range_value = _detail::index_range::Range <typename has_iterator <_T>::type>::calc(range);
    return IndexRange <decltype(range_value)>{range_value};
  }
}

template <class _Index>
class matsulib::IndexRange
{
protected:
  _Index _end;
  _Index _index;

public:
  // range = [begin, ..., end)
  IndexRange(_Index begin, _Index end) : _end{ end }, _index{ begin } {}
  // range = [0, ..., range)
  explicit IndexRange(_Index range) : _end{ range }, _index { 0 } {}

public:
  IndexRange() = delete;
  IndexRange(const IndexRange &) = default;
  IndexRange(IndexRange &&) = default;
  IndexRange &operator =(const IndexRange &) = default;
  IndexRange &operator =(IndexRange &&) = default;

public:
  auto operator *() -> _Index { return _index; }
  auto operator !=(IndexRange &) -> bool { return _index < _end; }
  auto operator ++() -> void { ++_index; }

  auto begin() const -> IndexRange { return *this; }
  auto end() const -> IndexRange { return *this; }
};