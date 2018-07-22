#ifndef JDB_RENDERER_HPP
#define JDB_RENDERER_HPP
#pragma once
#include "Shader_manager.hpp"
#include "Texture_manager.hpp"

namespace jdb
{
  class Mesh_renderer;

  class Renderer final
  {
  public:
    static void use_shader(Shader t_id);
    static void use_texture(Texture t_id);
    static void draw_mesh(const Mesh_renderer& t_mesh_renderer);

    static Shader shader();
  private:
    static Shader m_shader_program_id_;

    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer& t_to_copy) = default;
    Renderer(Renderer&& t_to_move) noexcept = default;
    Renderer& operator = (const Renderer& t_to_copy) = default;
    Renderer& operator = (Renderer&& t_to_move) noexcept = default;
  };
}//jdb

#endif JDB_RENDERER_HPP;