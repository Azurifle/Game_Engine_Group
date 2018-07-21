#ifndef G6037599_TEXTURE_HPP
#define G6037599_TEXTURE_HPP
#pragma once

namespace jdb
{
  using Texture = GLuint;

  class Texture_manager final
  {
  public:
    static Texture_manager get();
    Texture load_or_get(const std::string& t_png_file_path, GLenum t_color_format = GL_RGBA);
    void unload_all();

    ~Texture_manager() = default;
  private:
    Texture load(const std::string& t_png_file_path, GLenum t_color_format);

    std::map<std::string, Texture> m_texture_collection_{};

    Texture_manager() = default;
    Texture_manager(const Texture_manager& t_to_copy) = default;
    Texture_manager(Texture_manager&& t_to_move) = default;
    Texture_manager& operator = (const Texture_manager& t_to_copy) = default;
    Texture_manager& operator = (Texture_manager&& t_to_move) noexcept = default;
  };
}//G6037599

#endif G6037599_TEXTURE_HPP;