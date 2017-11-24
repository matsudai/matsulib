#pragma once

#include <vector>
#include <algorithm>

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
    auto operator[](const ValueArray <bool> &filtered_map) -> Assigner;

    operator std::vector <Type>() const;

  private:
    class Assigner
    {
    private:
      ValueArray & _destination;
      ValueArray <bool> & _filter;
    public:
      Assigner(ValueArray & destination, ValueArray <bool> &filter)
        : _destination{ destination }, _filter{ filter } {}
      Assigner() = delete;
      Assigner(const Assigner &) = delete;
      Assigner(Assigner &&) = delete;
      Assigner &operator=(const Assigner &) = delete;
      Assigner &operator=(Assigner &&) = delete;
      virtual ~Assigner() = default;
      auto operator=(ValueArray && source) -> ValueArray &;
      auto operator=(const Type & source) -> ValueArray &;
      operator ValueArray() const;
    };

    template <class BinaryOperation>
    auto zip(const ValueArray & values, BinaryOperation && op) const -> ValueArray;
    template <class UnaryOperation>
    auto zip(const Type & value, UnaryOperation && op) const-> ValueArray;

    template <class BinaryOperation>
    auto zip_destructive(const ValueArray & values, BinaryOperation && op) -> ValueArray &;
    template <class UnaryOperation>
    auto zip_destructive(const Type & value, UnaryOperation && op) -> ValueArray &;

    template <class BinaryOperation>
    auto where(BinaryOperation && op) const -> ValueArray <bool>;
    template <class UnaryOperation>
    auto where(UnaryOperation && op) const -> ValueArray <bool>;

    auto end_of_zipped_iterator(const ValueArray &compared) -> iterator;
    auto end_of_zipped_iterator(const ValueArray &compared) const -> const_iterator;
  };

  template <class T>
  inline auto ValueArray <T>::end_of_zipped_iterator(const ValueArray &compared) -> iterator
  {
    if (this->size() > compared.size())
    {
      return this->end() - (this->size() - compared.size());
    }
    return this->end();
  }
  template <class T>
  inline auto ValueArray <T>::end_of_zipped_iterator(const ValueArray &compared) const -> const_iterator
  {
    if (this->size() > compared.size())
    {
      return this->end() - (this->size() - compared.size());
    }
    return this->end();
  }

  template <class T> template <class BinaryOperation>
  inline auto ValueArray <T>::zip(const ValueArray & values, BinaryOperation && op) const -> ValueArray
  {
    ValueArray output = *this;
    std::transform(this->begin(), end_of_zipped_iterator(values), values.begin(), output.begin(), std::forward <BinaryOperation>(op));
    return std::move(output);
  }
  template <class T> template <class UnaryOperation>
  inline auto ValueArray <T>::zip(const T & value, UnaryOperation &&op) const -> ValueArray
  {
    ValueArray output = *this;
    std::transform(this->begin(), this->end(), output.begin(), std::forward <UnaryOperation>(op));
    return std::move(output);
  }

  template <class T> template <class BinaryOperation>
  inline auto ValueArray <T>::zip_destructive(const ValueArray &values, BinaryOperation &&op) -> ValueArray &
  {
    std::transform(this->begin(), end_of_zipped_iterator(values), values.begin(), this->begin(), std::forward <BinaryOperation>(op));
    return *this;
  }
  template <class T> template <class UnaryOperation>
  inline auto ValueArray <T>::zip_destructive(const T &value, UnaryOperation &&op) -> ValueArray &
  {
    std::transform(this->begin(), this->end(), this->begin(), std::forward <UnaryOperation>(op));
    return *this;
  }

  template <class T> template <class UnaryOperation>
  inline auto ValueArray <T>::where(UnaryOperation && op) const -> ValueArray <bool>
  {
    ValueArray <bool> selector;
    selector.resize(this->size());
    std::transform(this->begin(), this->end(), selector.begin(), std::forward <UnaryOperation>(op));
    return std::move(selector);
  }

  template <class T> auto ValueArray <T>::operator+(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::plus <T>{}));
  }
  template <class T> auto ValueArray <T>::operator-(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::minus <T>{}));
  }
  template <class T> auto ValueArray <T>::operator*(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::multiplies <T>{}));
  }
  template <class T> auto ValueArray <T>::operator/(const ValueArray & values) const -> ValueArray
  {
    return std::move(zip(values, std::divides <T>{}));
  }

  template <class T> auto ValueArray <T>::operator+(const T & value) const -> ValueArray
  {
    return std::move(zip(value, [&value](auto && value) {return std::move(value + value); }));
  }
  template <class T> auto ValueArray <T>::operator-(const T & value) const -> ValueArray
  {
    return std::move(zip(value, [&value](auto && value) {return std::move(value - value); }));
  }
  template <class T> auto ValueArray <T>::operator*(const T & value) const -> ValueArray
  {
    return std::move(zip(value, [&value](auto && value) {return std::move(value * value); }));
  }
  template <class T> auto ValueArray <T>::operator/(const T & value) const -> ValueArray
  {
    return std::move(zip(value, [&value](auto && value) {return std::move(value / value); }));
  }

  template <class T> auto ValueArray <T>::operator+=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::plus <T>{});
  }
  template <class T> auto ValueArray <T>::operator-=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::minus <T>{});
  }
  template <class T> auto ValueArray <T>::operator*=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::multiplies <T>{});
  }
  template <class T> auto ValueArray <T>::operator/=(const ValueArray & values) -> ValueArray &
  {
    return zip_destructive(values, std::divides <T>{});
  }

  template <class T> auto ValueArray <T>::operator+=(const T & value) -> ValueArray &
  {
    return zip_destructive(value, [&value](auto && value) {return std::move(value + value); });
  }
  template <class T> auto ValueArray <T>::operator-=(const T & value) -> ValueArray &
  {
    return zip_destructive(value, [&value](auto && value) {return std::move(value - value); });
  }
  template <class T> auto ValueArray <T>::operator*=(const T & value) -> ValueArray &
  {
    return zip_destructive(value, [&value](auto && value) {return std::move(value * value); });
  }
  template <class T> auto ValueArray <T>::operator/=(const T & value) -> ValueArray &
  {
    return zip_destructive(value, [&value](auto && value) {return std::move(value / value); });
  }

  template <class T> auto ValueArray <T>::operator<(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value < compared); });
  }
  template <class T> auto ValueArray <T>::operator>(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value > compared); });
  }
  template <class T> auto ValueArray <T>::operator==(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value == compared); });
  }
  template <class T> auto ValueArray <T>::operator!=(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value != compared); });
  }
  template <class T> auto ValueArray <T>::operator<=(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value <= compared); });
  }
  template <class T> auto ValueArray <T>::operator>=(const T & compared) const -> ValueArray <bool>
  {
    return where([&compared](auto && value) {return std::move(value >= compared); });
  }

  template <class T> template <class Function>
  inline auto ValueArray <T>::any(Function &&function) const -> bool
  {
    for (auto itr = this->begin(); itr != this->end(); ++itr)
    {
      if (static_cast <bool>(function(*itr))) { return true; }
    }
    return false;
  }
  template <class T>
  inline auto ValueArray <T>::any() const -> bool
  {
    for (auto itr = this->begin(); itr != this->end(); ++itr)
    {
      if (static_cast <bool>(*itr)) { return true; }
    }
    return false;
  }

  template <class T> template <class Function>
  inline auto ValueArray <T>::all(Function &&function) const -> bool
  {
    for (auto itr = this->begin(); itr != this->end(); ++itr)
    {
      if (!static_cast <bool>(function(*itr))) { return false; }
    }
    return true;
  }
  template <class T>
  inline auto ValueArray <T>::all() const -> bool
  {
    for (auto itr = this->begin(); itr != this->end(); ++itr)
    {
      if (!static_cast <bool>(*itr)) { return false; }
    }
    return true;
  }

  template <class T>
  inline auto ValueArray <T>::all_close(const ValueArray & values) -> bool
  {
    if (this->size() != values.size()) { return false; }
    auto size = this->size();
    for (std::size_t i = 0; i < size; ++i)
    {
      if ((*this)[i] != values[i]) { return false; }
    }
    return true;
  }
}