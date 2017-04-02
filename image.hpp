#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <emmintrin.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#include <intrin.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

//namespace matsulib::_detail
namespace matsulib { namespace _detail
{
#include "image/stb_image.h"
#include "image/stb_image_write.h"
}}

#include <vector>
#include <string>
#include <tuple>
#include <functional>

namespace matsulib
{
  struct Image final
  {
  public:
    int width = 0;
    int height = 0;
    int channel = 0;
    std::vector <unsigned char> pixels = {};
  };

  namespace image
  {
    enum class Component : int
    {
      NOT_SPECIFIED = 0,
      GRAY = 1,
      GRAY_ALPHA = 2,
      RGB = 3,
      RGBA = 4
    };
    enum class Format : int
    {
      NOT_SPECIFIED = 0,
      BMP = 1,
    };

    void write(const std::string &filename, const matsulib::Image &src, const Format fmt = Format::NOT_SPECIFIED)
    {
      std::function <int(const char *, int, int, int, const void *)> write;
      if (fmt == Format::NOT_SPECIFIED || fmt == Format::BMP)
      {
        write = _detail::stbi_write_bmp;
      }

      int result = write(filename.c_str(), src.width, src.height, src.channel, src.pixels.data());
      if (result == 0)
      {
        throw std::runtime_error{ "matsulib::image::write() : Could Not Write!!" };
      }
      return;
    }

    matsulib::Image read(const std::string &filename, const Component comp = Component::NOT_SPECIFIED)
    {
      int w, h, cmp;
      auto pixels = _detail::stbi_load(filename.c_str(), &w, &h, &cmp, static_cast <int>(comp));
      if (pixels == NULL)
      {
        throw std::runtime_error{ "matsulib::image::read() : Could Not Read!!" };
      }

      Image img/* = {w, h, static_cast <int>(comp), {}}*/;
      img.width = w;
      img.height = h;
      img.channel = static_cast <int>(comp);
      img.pixels.resize(img.width * img.height * img.channel);
      for (decltype(img.pixels.size()) i = 0; i < img.pixels.size(); i++)
      {
        img.pixels[i] = pixels[i];
      }
      std::free(pixels);
      return std::move(img);
    }
  }
}