#ifndef G6037599_TEXTURE_HPP
#define G6037599_TEXTURE_HPP
#pragma once

namespace G6037599
{
  using Texture = GLuint;

  class Texture_manager final
  {
  public:
    static Texture load_or_get(const std::string& t_png_file_path);

    ~Texture_manager() = default;
  private:
    static Texture load(const std::string& t_png_file_path
      , std::map<std::string, Texture>& t_texture_collection);

    std::map<std::string, Texture> m_texture_collection_{};

    Texture_manager() = default;
    Texture_manager(const Texture_manager& t_to_copy) = default;
    Texture_manager(Texture_manager&& t_to_move) = default;
    Texture_manager& operator = (const Texture_manager& t_to_copy) = default;
    Texture_manager& operator = (Texture_manager&& t_to_move) noexcept = default;
  };
}//G6037599

#endif G6037599_TEXTURE_HPP;