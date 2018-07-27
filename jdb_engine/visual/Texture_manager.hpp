#ifndef JDB_TEXTURE_HPP
#define JDB_TEXTURE_HPP
#pragma once

namespace jdb
{
  using Texture = GLuint;

  class Texture_manager final
  {
  public:
    static Texture load_or_get(const std::string& t_png_file_path, GLenum t_color_format = GL_RGB);

    ~Texture_manager() = default;
  private:
    Texture load(const std::string& t_png_file_path, GLenum t_color_format);

    std::map<std::string, Texture> m_loadeds_{};

    Texture_manager() = default;
    Texture_manager(const Texture_manager& t_to_copy) = default;
    Texture_manager(Texture_manager&& t_to_move) = default;
    Texture_manager& operator = (const Texture_manager& t_to_copy) = default;
    Texture_manager& operator = (Texture_manager&& t_to_move) noexcept = default;
  };
}//jdb

#endif JDB_TEXTURE_HPP;