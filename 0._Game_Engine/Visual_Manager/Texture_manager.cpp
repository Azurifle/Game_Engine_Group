#include <stdafx.h>
#include "Texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Thirdparty/stb_image.h"//can't add include guard

namespace jdb
{
  // ___ public _______________________________________________________________________________

  Texture_manager Texture_manager::get()
  {
    static const Texture_manager INSTANCE;
    return INSTANCE;
  }

  Texture Texture_manager::load_or_get(const std::string& t_png_file_path)
  {
    //static Texture_manager instance;

    const auto FOUND_TEXTURE = m_texture_collection_.find(t_png_file_path);
    if (FOUND_TEXTURE != m_texture_collection_.end())
    {
      return FOUND_TEXTURE->second;
    }

    //std::cout << m_texture_collection_.size() << std::endl;

    return load(t_png_file_path);
  }

  void Texture_manager::unload_all()
  {
    m_texture_collection_.clear();
  }

  // ___ private _________________________________________________________________________________

  Texture Texture_manager::load(const std::string& t_png_file_path)
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
    free(TEXTURE_IMG);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    m_texture_collection_[t_png_file_path] = id;
    //std::cout << m_texture_collection_[id] << "," << m_texture_collection_[t_png_file_path] << std::endl;//****

    return id;
  }
}//jdb