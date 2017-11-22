#pragma once

#include <vector>

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
    ValueArray &operator=(const ValueArray &) = default;
    ValueArray &operator=(ValueArray &&) noexcept = default;
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
    
    auto operator<(const Type & compared) const -> ValueArray <bool>;
    auto operator>(const Type & compared) const -> ValueArray <bool>;
    auto operator==(const Type & compared) const -> ValueArray <bool>;
    auto operator!=(const Type & compared) const -> ValueArray <bool>;
    auto operator<=(const Type & compared) const -> ValueArray <bool>;
    auto operator>=(const Type & compared) const -> ValueArray <bool>;

    template <class Function> auto any(Function &&function) -> bool;
    template <class Function> auto any() -> bool;

    template <class Function> auto all(Function &&function) -> bool;
    template <class Function> auto all() -> bool;

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
  };
}