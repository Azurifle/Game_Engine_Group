#include <stdafx.h>
#include "Renderer.hpp"
#include "Mesh_renderer.hpp"

namespace G6037599
{
  // ___ static __________________________________________________________________________________

  GLuint Renderer::m_shader_program_id_ = 0;

  void Renderer::use_shader(const GLuint t_shader_id)
  {
    m_shader_program_id_ = t_shader_id;
    glUseProgram(m_shader_program_id_);
  }

  void Renderer::use_texture(const GLuint t_texture_id)
  {
    glBindTexture(GL_TEXTURE_2D, t_texture_id);
  }

  void Renderer::draw_mesh(const Mesh_renderer& t_mesh_renderer)
  {
    glBindVertexArray(t_mesh_renderer.id());
  }

  GLuint Renderer::shader()
  {
    return m_shader_program_id_;
  }
}//G6037599