#pragma once

#include <vector>
#include <algorithm>
#include <functional>

namespace matsulib {
  template <class Type>
  class ValueArray
    : public std::vector <Type>
  {
  public:
    using parent = std::vector <Type>;

    using parent::parent;

    ValueArray() = default;
    ValueArray(const ValueArray &) = default;
    ValueArray(ValueArray &&) noexcept = default;
    ValueArray &operator=(const ValueArray &) & = default;
    ValueArray &operator=(ValueArray &&) & noexcept = default;
    virtual ~ValueArray() = default;

    auto operator+(const ValueArray & values) const -> ValueArray;
    auto operator-(const ValueArray & values) const -> ValueArray;
    auto operator*(const ValueArray & values) const -> ValueArray;
    auto operator/(const ValueArray & values) const -> ValueArray;

    auto operator+(const Type & value) const -> ValueArray;
    auto operator-(const Type & value) const -> ValueArray;
    auto operator*(const Type & value) const -> ValueArray;
    auto operator/(const Type & value) const -> ValueArray;

    auto operator+=(const ValueArray & values) -> ValueArray &;
    auto operator-=(const ValueArray & values) -> ValueArray &;
    auto operator*=(const ValueArray & values) -> ValueArray &;
    auto operator/=(const ValueArray & values) -> ValueArray &;
    
    auto operator+=(const Type & value) -> ValueArray &;
    auto operator-=(const Type & value) -> ValueArray &;
    auto operator*=(const Type & value) -> ValueArray &;
    auto operator/=(const Type & value) -> ValueArray &;
    
    auto operator<(const ValueArray & compared) const -> ValueArray <bool>;
    auto operator>(const ValueArray & compared) const -> ValueArray <bool>;
    auto operator==(const ValueArray & compared) const -> ValueArray <bool>;
    auto operator!=(const ValueArray & compared) const -> ValueArray <bool>;
    auto operator<=(const ValueArray & compared) const -> ValueArray <bool>;
    auto operator>=(const ValueArray & compared) const -> ValueArray <bool>;

    auto operator<(const Type & compared) const -> ValueArray <bool>;
    auto operator>(const Type & compared) const -> ValueArray <bool>;
    auto operator==(const Type & compared) const -> ValueArray <bool>;
    auto operator!=(const Type & compared) const -> ValueArray <bool>;
    auto operator<=(const Type & compared) const -> ValueArray <bool>;
    auto operator>=(const Type & compared) const -> ValueArray <bool>;

    template <class Function> auto any(Function && function) const -> bool;
    auto any() const -> bool;

    template <class Function> auto all(Function && function) const -> bool;
    auto all() const -> bool;

    auto all_close(const ValueArray & values) -> bool;

  private:
    class Assigner;

  public:
    using parent::operator[];
    auto operator[](const ValueArray <bool> & selector) -> Assigner;

    operator std::vector <Type>() const;

  private:
    class Assigner
    {
    private:
      ValueArray & _source;
      std::vector <std::size_t> _selected_indexes;
    public:
      Assigner(ValueArray & source, const ValueArray <bool> & selector);
      Assigner() = delete;
      Assigner(const Assigner &) = delete;
      Assigner(Assigner &&) noexcept = default;
      Assigner &operator=(const Assigner &) = delete;
      Assigner &operator=(Assigner &&) noexcept = delete;
      virtual ~Assigner() = default;
      auto operator=(ValueArray && values) && -> ValueArray &;
      auto operator=(const Type & value) && -> ValueArray &;
      auto to_value_array() const -> ValueArray;
      operator ValueArray() const;
    };

    template <class BinaryOperation>
    auto zip(const ValueArray & values, BinaryOperation && op) const -> ValueArray;
    template <class UnaryOperation>
    auto zip(UnaryOperation && op) const-> ValueArray;

    template <class BinaryOperation>
    auto zip_destructive(const ValueArray & values, BinaryOperation && op) -> ValueArray &;
    template <class UnaryOperation>
    auto zip_destructive(UnaryOperation && op) -> ValueArray &;

    template <class BinaryOperation>
    auto where(const ValueArray & values, BinaryOperation && op) const -> ValueArray <bool>;
    template <class UnaryOperation>
    auto where(UnaryOperation && op) const -> ValueArray <bool>;

    template <class Type2>
    auto end_of_zipped_iterator(const ValueArray <Type2> &compared) -> iterator;
    template <class Type2>
    auto end_of_zipped_iterator(const ValueArray <Type2> &compared) const -> const_iterator;
  };

  template <class T> template <class T2>
  inline auto ValueArray <T>::end_of_zipped_iterator(const ValueArray <T2> &compared) -> iterator
  {
    if (size() > compared.size())
    {
      return end() - (size() - compared.size());
    }
    return end();
  }
  template <class T> template <class T2>
  inline auto ValueArray <T>::end_of_zipped_iterator(const ValueArray <T2> &compared) const -> const_iterator
  {
    if (size() > compared.size())
    {
      return end() - (size() - compared.size());
    }
    return end();
  }

  template <class T> template <class BinaryOperation>
  inline auto ValueArray <T>::zip(const ValueArray & values, BinaryOperation && op) const -> ValueArray
  {
    ValueArray output = *this;
    std::transform(begin(), end_of_zipped_iterator(values), values.begin(), output.begin(), std::forward <BinaryOperation>(op));
    return std::move(output);
  }
  template <class T> template <class UnaryOperation>
  inline auto ValueArray <T>::zip(UnaryOperation &&op) const -> ValueArray
  {
    ValueArray output = *this;
    std::transform(begin(), end(), output.begin(), std::forward <UnaryOperation>(op));
    return std::move(output);
  }

  template <class T> template <class BinaryOperation>
  inline auto ValueArray <T>::zip_destructive(const ValueArray &values, BinaryOperation &&op) -> ValueArray &
  {
    std::transform(begin(), end_of_zipped_iterator(values), values.begin(), begin(), std::forward <BinaryOperation>(op));
    return *this;
  }
  template <class T> template <class UnaryOperation>
  inline auto ValueArray <T>::zip_destructive(UnaryOperation &&op) -> ValueArray &
  {
    std::transform(begin(), end(), begin(), std::forward <UnaryOperation>(op));
    return *this;
  }

  template <class T> template <class BinaryOperation>
  inline auto ValueArray <T>::where(const ValueArray & values, BinaryOperation && op) const -> ValueArray <bool>
  {
    ValueArray <bool> selector;
    selector.resize(size());
    auto end_of_itr = end_of_zipped_iterator(values);
    std::transform(begin(), end_of_itr, values.begin(), selector.begin(), std::forward <BinaryOperation>(op));
    auto end_of_selector = selector.end();
    std::fill(end_of_selector - (end() - end_of_itr), end_of_selector, false);
    return std::move(selector);
  }
  template <class T> template <class UnaryOperation>
  inline auto ValueArray <T>::where(UnaryOperation && op) const -> ValueArray <bool>
  {
    ValueArray <bool> selector;
    selector.resize(size());
    std::transform(begin(), end(), selector.begin(), std::forward <UnaryOperation>(op));
    return std::move(selector);
  }

  template <class T> inline auto ValueArray <T>::operator+(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::plus <T>{}));
  }
  template <class T> inline auto ValueArray <T>::operator-(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::minus <T>{}));
  }
  template <class T> inline auto ValueArray <T>::operator*(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::multiplies <T>{}));
  }
  template <class T> inline auto ValueArray <T>::operator/(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::divides <T>{}));
  }

  template <class T> inline auto ValueArray <T>::operator+(const T & value) const -> ValueArray
  {
    return std::move(zip([&value](auto && src_value) {return std::move(src_value + value); }));
  }
  template <class T> inline auto ValueArray <T>::operator-(const T & value) const -> ValueArray
  {
    return std::move(zip([&value](auto && src_value) {return std::move(src_value - value); }));
  }
  template <class T> inline auto ValueArray <T>::operator*(const T & value) const -> ValueArray
  {
    return std::move(zip([&value](auto && src_value) {return std::move(src_value * value); }));
  }
  template <class T> inline auto ValueArray <T>::operator/(const T & value) const -> ValueArray
  {
    return std::move(zip([&value](auto && src_value) {return std::move(src_value / value); }));
  }

  template <class T> inline auto ValueArray <T>::operator+=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::plus <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator-=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::minus <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator*=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::multiplies <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator/=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::divides <T>{});
  }

  template <class T> inline auto ValueArray <T>::operator+=(const T & value) -> ValueArray &
  {
    return zip_destructive([&value](auto && src_value) {return std::move(src_value + value); });
  }
  template <class T> inline auto ValueArray <T>::operator-=(const T & value) -> ValueArray &
  {
    return zip_destructive([&value](auto && src_value) {return std::move(src_value - value); });
  }
  template <class T> inline auto ValueArray <T>::operator*=(const T & value) -> ValueArray &
  {
    return zip_destructive([&value](auto && src_value) {return std::move(src_value * value); });
  }
  template <class T> inline auto ValueArray <T>::operator/=(const T & value) -> ValueArray &
  {
    return zip_destructive([&value](auto && src_value) {return std::move(src_value / value); });
  }

  template <class T> inline auto ValueArray <T>::operator<(const ValueArray & compared) const -> ValueArray <bool>
  {
    return where(compared, std::less <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator>(const ValueArray & compared) const -> ValueArray <bool>
  {
    return where(compared, std::greater <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator==(const ValueArray & compared) const -> ValueArray <bool>
  {
    return where(compared, std::equal_to <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator!=(const ValueArray & compared) const -> ValueArray <bool>
  {
    return where(compared, std::not_equal_to <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator<=(const ValueArray & compared) const -> ValueArray <bool>
  {
    return where(compared, std::less_equal <T>{});
  }
  template <class T> inline auto ValueArray <T>::operator>=(const ValueArray & compared) const -> ValueArray <bool>
  {
    return where(compared, std::greater_equal <T>{});
  }

  template <class T> inline auto ValueArray <T>::operator<(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value < compared); });
  }
  template <class T> inline auto ValueArray <T>::operator>(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value > compared); });
  }
  template <class T> inline auto ValueArray <T>::operator==(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value == compared); });
  }
  template <class T> inline auto ValueArray <T>::operator!=(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value != compared); });
  }
  template <class T> inline auto ValueArray <T>::operator<=(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value <= compared); });
  }
  template <class T> inline auto ValueArray <T>::operator>=(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value >= compared); });
  }

  template <class T> template <class Function>
  inline auto ValueArray <T>::any(Function && function) const -> bool
  {
    return std::any_of(this->begin(), this->end(), [&function](auto && v) { return static_cast <bool>(function(v)); });
  }
  template <class T>
  inline auto ValueArray <T>::any() const -> bool
  {
    return std::any_of(this->begin(), this->end(), [](auto && v) { return static_cast <bool>(v); });
  }

  template <class T> template <class Function>
  inline auto ValueArray <T>::all(Function && function) const -> bool
  {
    return std::all_of(this->begin(), this->end(), [&function](auto && v) { return static_cast <bool>(function(v)); });;
  }
  template <class T>
  inline auto ValueArray <T>::all() const -> bool
  {
    return std::all_of(this->begin(), this->end(), [](auto && v) { return static_cast <bool>(v); });
  }

  template <class T>
  inline auto ValueArray <T>::all_close(const ValueArray & values) -> bool
  {
    if (size() != values.size()) { return false; }
    auto loop_count = size();
    for (std::size_t i = 0; i < loop_count; ++i)
    {
      if ((*this)[i] != values[i]) { return false; }
    }
    return true;
  }

  template <class T>
  inline auto ValueArray <T>::operator[](const ValueArray <bool> & selector) -> Assigner
  {
    return Assigner(*this, selector);
  }

  template <class T>
  inline ValueArray <T>::Assigner::Assigner(ValueArray & destination, const ValueArray <bool> & selector) : _source{ destination }
  {
    _selected_indexes.resize(std::count(selector.begin(), selector.end(), true));
    auto num_of_loop = selector.size();
    auto size_of_source = _source.size();
    for (std::size_t i = 0, j = 0; i < num_of_loop; ++i)
    {
      if (i >= size_of_source) { return; }
      if (!selector[i]) { continue; }
      _selected_indexes[j] = i; ++j;
    }
  }

  template <class T>
  inline auto ValueArray <T>::Assigner::operator=(const T & value) && -> ValueArray &
  {
    auto size_of_source = _source.size();
    for (auto i : _selected_indexes)
    {
      _source[i] = value;
    }
    return _source;
  }

  template <class T>
  inline auto ValueArray <T>::Assigner::to_value_array() const -> ValueArray
  {
    ValueArray selected_values;
    selected_values.reserve(_selected_indexes.size());
    for (auto i : _selected_indexes)
    {
      selected_values.push_back(_source[i]);
    }
    return std::move(selected_values);
  }

  template <class T>
  inline ValueArray <T>::Assigner::operator ValueArray <T>() const
  {
    return std::move(to_value_array());
  }
}