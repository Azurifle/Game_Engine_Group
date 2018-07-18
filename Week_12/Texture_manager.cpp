#include <stdafx.h>
#include "Texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Thirdparty/stb_image.h"//can't add include guard

namespace G6037599
{
  // ___ public _______________________________________________________________________________

  Texture Texture_manager::load_or_get(const std::string& t_png_file_path)
  {
    static Texture_manager instance;

    const auto FOUND_TEXTURE = instance.m_texture_collection_.find(t_png_file_path);
    if (FOUND_TEXTURE != instance.m_texture_collection_.end())
    {
      return FOUND_TEXTURE->second;
    }

    return load(t_png_file_path, instance.m_texture_collection_);
  }

  // ___ private _________________________________________________________________________________

  Texture Texture_manager::load(const std::string& t_png_file_path
    , std::map<std::string, Texture>& t_texture_collection)
  {
    REQUIRE(!t_png_file_path.empty());
    Texture id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int img_width, img_height, components;
    const auto TEXTURE_IMG = stbi_load(t_png_file_path.c_str()
      , &img_width, &img_height, &components, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, img_width, img_height,
      0, GL_RGBA, GL_UNSIGNED_BYTE, TEXTURE_IMG);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    t_texture_collection[t_png_file_path] = id;

    return id;
  }
}//G6037599