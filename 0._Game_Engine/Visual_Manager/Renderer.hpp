#ifndef G6037599_RENDERER_HPP
#define G6037599_RENDERER_HPP
#pragma once
#include "Shader.hpp"

namespace G6037599
{
  class Mesh_renderer;

  class Renderer final
  {
  public:
    static void use_shader(GLuint t_shader_id);
    static void use_texture(GLuint t_texture_id);
    static void draw_mesh(const Mesh_renderer& t_mesh_renderer);

    static GLuint shader();
  private:
    static GLuint m_shader_program_id_;

    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer& t_to_copy) = default;
    Renderer(Renderer&& t_to_move) noexcept = default;
    Renderer& operator = (const Renderer& t_to_copy) = default;
    Renderer& operator = (Renderer&& t_to_move) noexcept = default;
  };
}//G6037599

#endif G6037599_RENDERER_HPP;