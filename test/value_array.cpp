#define CATCH_CONFIG_MAIN
#include "catch2/single_include/catch.hpp"

#include "../value_array.hpp"
#include <string>

TEST_CASE("インスタンスの生成", "[matsulib::ValueArray]")
{
  SECTION("デフォルトコンストラクタ")
  {
    matsulib::ValueArray <int> a;
    CHECK(a.size() == 0);
    CHECK(a.empty() == true);
  }

  SECTION("std::vectorのコンストラクタ")
  {
    matsulib::ValueArray <int> b(3);
    CHECK(b.size() == 3);
    
    matsulib::ValueArray <int> c(3, 2);
    CHECK(c[2] == 2);

    matsulib::ValueArray <int> d{ 1, 2, 3 };
    CHECK(d[2] == 3);

    matsulib::ValueArray <std::string> s{ "hoge", "piyo", "fuga" };
    CHECK(s[2] == "fuga");
  }
}

TEST_CASE("比較演算子とか", "[matsulib::ValueArray]")
{
  SECTION("All OK?")
  {
    matsulib::ValueArray <bool> a{ true, true, true };
    CHECK(a.all());
    CHECK(a.all([](auto && v) { return v == true; }));

    matsulib::ValueArray <bool> b{ true, false, true };
    CHECK_FALSE(b.all());
    CHECK_FALSE(b.all([](auto && v) { return v == true; }));

    matsulib::ValueArray <int> c{ 1, 2, 3 };
    //CHECK(c.all());
    CHECK(c.all([](auto && v) { return v < 4 && v > 0; }));

    matsulib::ValueArray <int> d{ 0, 1, 2 };
    //CHECK_FALSE(d.all());
    CHECK_FALSE(d.all([](auto && v) { return v < 1; }));
  }
  SECTION("Any OK?")
  {
    matsulib::ValueArray <bool> a{ true, true, true };
    CHECK(a.any());
    CHECK(a.any([](auto && v) { return v == true; }));

    matsulib::ValueArray <bool> b{ true, false, true };
    CHECK(b.any());
    CHECK(b.any([](auto && v) { return v == true; }));

    matsulib::ValueArray <bool> c{ false, false, false };
    //CHECK_FALSE(c.any());
    CHECK_FALSE(c.any([](auto && v) { return v == true; }));

    matsulib::ValueArray <int> d{ 0, 1, 2 };
    CHECK(d.any([](auto && v) { return v == 0; }));
    CHECK_FALSE(d.any([](auto && v) { return v > 2; }));
  }

  SECTION("2つの配列の要素の値がすべて等しい")
  {
    matsulib::ValueArray <int> a{ 0, 2, 4, 6 };
    matsulib::ValueArray <int> b{ 0, 2, 4, 6 };
    CHECK(a.all_close(a));
    CHECK(a.all_close(b));
    CHECK(a.all_close({ 0, 2, 4, 6 }));

    matsulib::ValueArray <int> c{ 0, 2, 4, 7 };
    CHECK_FALSE(a.all_close(c));
    c = { 0, 2, 4, 6, 0 };
    CHECK_FALSE(a.all_close(c));
    c = { 0, 2, 4 };
    CHECK_FALSE(a.all_close(c));
  }

  SECTION("値との比較")
  {
    SECTION("==")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      CHECK((a == 2).all_close({ false, false, true, false }));

      matsulib::ValueArray <std::string> s{ "hoge", "fuga", "moge", "piyo" };
      CHECK((s == "moge").all_close({ false, false, true, false }));
    }

    SECTION("!=")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      CHECK((a != 2).all_close({ true, true, false, true }));
    }

    SECTION(">")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      CHECK((a > 2).all_close({ false, false, false, true }));
    }

    SECTION(">=")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      CHECK((a >= 2).all_close({ false, false, true, true }));
    }

    SECTION("<")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      CHECK((a < 2).all_close({ true, true, false, false }));
    }

    SECTION("<=")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      CHECK((a <= 2).all_close({ true, true, true, false }));
    }

  }

  SECTION("配列との比較")
  {
    matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
    matsulib::ValueArray <int> b{ -1, 0, 2, 4 };
    matsulib::ValueArray <int> c{ -1, 0 };
    matsulib::ValueArray <int> d{ -1, 0, 2, 4, 5 };
    SECTION("==")
    {
      CHECK((a == b).all_close({ false, false, true, false }));
      CHECK((a == c).all_close({ false, false, false, false }));
      CHECK((a == d).all_close({ false, false, true, false }));

      matsulib::ValueArray <std::string> s{ "hoge", "fuga", "moge", "piyo" };
      matsulib::ValueArray <std::string> c{ "hogee", "fugo", "moge", "piy" };
      CHECK((s == c).all_close({ false, false, true, false }));
    }

    SECTION("!=")
    {
      CHECK((a != b).all_close({ true, true, false, true }));
      CHECK((a != c).all_close({ true, true, false, false }));
      CHECK((a != d).all_close({ true, true, false, true }));
    }

    SECTION(">")
    {
      CHECK((a > b).all_close({ true, true, false, false }));
      CHECK((a > c).all_close({ true, true, false, false }));
      CHECK((a > d).all_close({ true, true, false, false }));
    }

    SECTION(">=")
    {
      CHECK((a >= b).all_close({ true, true, true, false }));
      CHECK((a >= c).all_close({ true, true, false, false }));
      CHECK((a >= d).all_close({ true, true, true, false }));
    }

    SECTION("<")
    {
      CHECK((a < b).all_close({ false, false, false, true }));
      CHECK((a < c).all_close({ false, false, false, false }));
      CHECK((a < d).all_close({ false, false, false, true }));
    }

    SECTION("<=")
    {
      CHECK((a <= b).all_close({ false, false, true, true }));
      CHECK((a <= c).all_close({ false, false, false, false }));
      CHECK((a <= d).all_close({ false, false, true, true }));
    }
  }
}

TEST_CASE("加減乗除", "[matsulib::ValueArray]")
{
  SECTION("非破壊的変更")
  {
    SECTION("値との比較")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      SECTION("+")
      {
        CHECK((a + 2).all_close({ 2, 3, 4, 5 }));
        
        matsulib::ValueArray <std::string> s1{ "ho", "fu", "pi" };
        CHECK((s1 + "ge").all_close({ "hoge", "fuge", "pige" }));
      }

      SECTION("-")
      {
        CHECK((a - 2).all_close({ -2, -1, 0, 1 }));
      }

      SECTION("*")
      {
        CHECK((a * 2).all_close({ 0, 2, 4, 6 }));
      }

      SECTION("/")
      {
        CHECK((a / 2).all_close({ 0, 0, 1, 1 }));
      }
    }
    SECTION("配列との比較")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      matsulib::ValueArray <int> b{ 4, 3, 2, 1 };
      matsulib::ValueArray <int> c{ 4, 3, 2 };
      matsulib::ValueArray <int> d{ 4, 3, 2, 1, 0 };
      SECTION("+")
      {
        CHECK((a + b).all_close({ 4, 4, 4, 4 }));
        CHECK((a + c).all_close({ 4, 4, 4, 3 }));
        CHECK((a + d).all_close({ 4, 4, 4, 4 }));

        matsulib::ValueArray <std::string> s1{ "ho", "fu", "pi" };
        matsulib::ValueArray <std::string> s2{ "ge", "ga", "yo" };
        CHECK((s1 + s2).all_close({ "hoge", "fuga", "piyo" }));
      }

      SECTION("-")
      {
        CHECK((a - b).all_close({ -4, -2, 0, 2 }));
        CHECK((a - c).all_close({ -4, -2, 0, 3 }));
        CHECK((a - d).all_close({ -4, -2, 0, 2 }));
      }

      SECTION("*")
      {
        CHECK((a * b).all_close({ 0, 3, 4, 3 }));
        CHECK((a * c).all_close({ 0, 3, 4, 3 }));
        CHECK((a * d).all_close({ 0, 3, 4, 3 }));
      }

      SECTION("/")
      {
        CHECK((a / b).all_close({ 0, 0, 1, 3 }));
        CHECK((a / c).all_close({ 0, 0, 1, 3 }));
        CHECK((a / d).all_close({ 0, 0, 1, 3 }));
      }
    }
  }
  SECTION("破壊的変更")
  {
    SECTION("値との比較")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      SECTION("+")
      {
        auto v = a;
        v += 2;
        CHECK(v.all_close({ 2, 3, 4, 5 }));

        matsulib::ValueArray <std::string> s1{ "ho", "fu", "pi" };
        CHECK((s1 + "ge").all_close({ "hoge", "fuge", "pige" }));
      }

      SECTION("-")
      {
        auto v = a;
        v -= 2;
        CHECK(v.all_close({ -2, -1, 0, 1 }));
      }

      SECTION("*")
      {
        auto v = a;
        v *= 2;
        CHECK(v.all_close({ 0, 2, 4, 6 }));
      }

      SECTION("/")
      {
        auto v = a;
        v /= 2;
        CHECK(v.all_close({ 0, 0, 1, 1 }));
      }
    }
    SECTION("配列との比較")
    {
      matsulib::ValueArray <int> a{ 0, 1, 2, 3 };
      matsulib::ValueArray <int> b{ 4, 3, 2, 1 };
      matsulib::ValueArray <int> c{ 4, 3, 2 };
      matsulib::ValueArray <int> d{ 4, 3, 2, 1, 0 };
      SECTION("+")
      {
        auto v = a;
        v += b;
        CHECK(v.all_close({ 4, 4, 4, 4 }));
        v = a;
        v += c;
        CHECK(v.all_close({ 4, 4, 4, 3 }));
        v = a;
        v += d;
        CHECK(v.all_close({ 4, 4, 4, 4 }));

        matsulib::ValueArray <std::string> s1{ "ho", "fu", "pi" };
        matsulib::ValueArray <std::string> s2{ "ge", "ga", "yo" };
        s1 += s2;
        CHECK(s1.all_close({ "hoge", "fuga", "piyo" }));
      }

      SECTION("-")
      {
        auto v = a;
        v -= b;
        CHECK(v.all_close({ -4, -2, 0, 2 }));
        v = a;
        v -= c;
        CHECK(v.all_close({ -4, -2, 0, 3 }));
        v = a;
        v -= d;
        CHECK(v.all_close({ -4, -2, 0, 2 }));
      }

      SECTION("*")
      {
        auto v = a;
        v *= b;
        CHECK(v.all_close({ 0, 3, 4, 3 }));
        v = a;
        v *= c;
        CHECK(v.all_close({ 0, 3, 4, 3 }));
        v = a;
        v *= d;
        CHECK(v.all_close({ 0, 3, 4, 3 }));
      }

      SECTION("/")
      {
        auto v = a;
        v /= b;
        CHECK(v.all_close({ 0, 0, 1, 3 }));
        v = a;
        v /= c;
        CHECK(v.all_close({ 0, 0, 1, 3 }));
        v = a;
        v /= d;
        CHECK(v.all_close({ 0, 0, 1, 3 }));
      }
    }
  }
}

TEST_CASE("添え字アクセス演算子", "[matsulib::ValueArray]")
{
  matsulib::ValueArray <int> a{ 0, 1, 2, 3, 4 };
  SECTION("数字でアクセス")
  {
    CHECK(a[2] == 2);
  }

  SECTION("Binary Arrayでアクセス")
  {
    auto v = a;
    v[a < 3] = 10;
    CHECK(v.all_close({ 10, 10, 10, 3, 4 }));

    auto u = a[a < 3].to_value_array();
    CHECK(u.all_close({ 0, 1, 2 }));
    auto s = static_cast <matsulib::ValueArray <int>>(a[a < 3]);
    CHECK(s.all_close({ 0, 1, 2 }));
  }
}