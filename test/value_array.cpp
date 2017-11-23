#define CATCH_CONFIG_MAIN
#include "catch2/single_include/catch.hpp"

#include "../value_array.hpp"
#include <string>

TEST_CASE("�C���X�^���X�̐���", "[matsulib::ValueArray]")
{
  SECTION("�f�t�H���g�R���X�g���N�^")
  {
    matsulib::ValueArray <int> a;
    CHECK(a.size() == 0);
    CHECK(a.empty() == true);
  }

  SECTION("std::vector�̃R���X�g���N�^")
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

TEST_CASE("True or False?", "[matsulib::ValueArray]")
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
    CHECK(c.all());
    CHECK(c.all([](auto && v) { return v < 4 && v > 0; }));

    matsulib::ValueArray <int> d{ 0, 1, 2 };
    CHECK_FALSE(d.all());
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
    CHECK_FALSE(c.any());
    CHECK_FALSE(c.any([](auto && v) { return v == true; }));

    matsulib::ValueArray <int> d{ 0, 1, 2 };
    CHECK(d.any([](auto && v) { return v == 0; }));
    CHECK_FALSE(d.any([](auto && v) { return v > 2; }));
  }

  SECTION("2�̔z��̗v�f�̒l�����ׂē�����")
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

  SECTION("== value")
  {

  }

  SECTION("!= value")
  {

  }

  SECTION("> value")
  {

  }

  SECTION(">= value")
  {

  }

  SECTION("< value")
  {

  }

  SECTION("<= value")
  {

  }
}

TEST_CASE("�����揜", "[matsulib::ValueArray]")
{
  SECTION("��j��I�ύX")
  {
    SECTION("���Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }

    SECTION("���Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }

    SECTION("��Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }

    SECTION("���Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }
  }
  SECTION("�j��I�ύX")
  {
    SECTION("���Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }

    SECTION("���Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }

    SECTION("��Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }

    SECTION("���Z")
    {
      SECTION("1�̒l")
      {

      }
      SECTION("�z��")
      {

      }
    }
  }
}