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
namespace matsulib { namespace image { namespace _detail
{
#include "image/stb_image.h"
#include "image/stb_image_write.h"
}}}

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

    auto parse(const matsulib::Image &src_img) -> std::vector <matsulib::Image>
    {
      auto src_img_size = src_img.pixels.size();
      auto num_of_channel = src_img.channel;
      std::vector <matsulib::Image> dst_imgs(num_of_channel);
      auto dst_img_size = num_of_channel == 0 ? 0 : src_img_size / num_of_channel;
      for (auto &dst_img : dst_imgs)
      {
        dst_img.channel = 1;
        dst_img.width = src_img.width;
        dst_img.height = src_img.height;
        dst_img.pixels.resize(dst_img_size);
      }
      for (decltype(num_of_channel) channel = 0; channel < num_of_channel; channel++)
      {
        for (decltype(dst_img_size) i = 0; i < dst_img_size; i++)
        {
          dst_imgs[channel].pixels[i] = src_img.pixels[i * num_of_channel + channel];
        }
      }
      return dst_imgs;
    }

    namespace _detail
    {
      inline auto merge_impl(matsulib::Image &dst, int channel_index, const matsulib::Image &src) -> void
      {
        auto img_size = dst.pixels.size() / dst.channel < src.pixels.size() ? dst.pixels.size() / dst.channel : src.pixels.size();
        for (decltype(img_size) i = 0; i < img_size; i++)
        {
          dst.pixels[i * dst.channel + channel_index] = src.pixels[i];
        }
      }
      template <class ..._Images>
      inline auto merge_impl(matsulib::Image &dst, int channel_index, const matsulib::Image &src_first, const _Images & ...src_args) -> void
      {
        merge_impl(dst, channel_index, src_first);
        merge_impl(dst, channel_index + 1, src_args...);
      }
    }

    template <class ..._Images>
    auto merge(const matsulib::Image &first, const _Images & ...args) -> matsulib::Image
    {
      matsulib::Image dst_img;
      dst_img.channel = 1 + sizeof...(args);
      dst_img.width = first.width;
      dst_img.height = first.height;
      dst_img.pixels.resize(first.pixels.size() * dst_img.channel);

      _detail::merge_impl(dst_img, 0, first, args...);
      return dst_img;
    }

    auto merge(const std::vector <matsulib::Image> &src_imgs) -> matsulib::Image
    {
      matsulib::Image dst_img;
      if (src_imgs.size() == 0)
      {
        dst_img.width = dst_img.height = dst_img.channel = 0;
      }
      auto num_of_channel = src_imgs.size();
      dst_img.channel = static_cast <decltype(dst_img.channel)>(num_of_channel);
      dst_img.width = src_imgs[0].width;
      dst_img.height = src_imgs[0].height;
      dst_img.pixels.resize(src_imgs[0].pixels.size() * dst_img.channel);
      for (decltype(num_of_channel) channel = 0; channel < num_of_channel; channel++)
      {
        _detail::merge_impl(dst_img, static_cast <int>(channel), src_imgs[channel]);
      }
      return dst_img;
    }

    auto write(const std::string &filename, const matsulib::Image &src, const Format fmt = Format::NOT_SPECIFIED) -> void
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

    auto rectangle(const matsulib::Image &src, decltype(matsulib::Image::width) beg_x, decltype(matsulib::Image::height) beg_y, decltype(matsulib::Image::width) width, decltype(matsulib::Image::height) height) -> matsulib::Image
    {
      matsulib::Image dst;
      dst.channel = src.channel;
      dst.width = width;
      dst.height = height;
      dst.pixels.resize(width * height * dst.channel);
      for (decltype(height) y = 0; y < height; y++) {
        for (decltype(width) x = 0; x < width; x++)
        {
          auto dst_index = (y * width + x) * dst.channel;
          auto src_index = ((y + beg_y) * src.width + (x + beg_x)) * src.channel;
          for (decltype(dst.channel) channel = 0; channel < dst.channel; channel++)
          {
            dst.pixels[dst_index + channel] = src.pixels[src_index + channel];
          }
        }
      }
      return dst;
    }

    auto read(const std::string &filename, const Component comp = Component::NOT_SPECIFIED) -> matsulib::Image
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