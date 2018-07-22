#include <stdafx.h>
#include "Renderer.hpp"
#include "Mesh_renderer.hpp"

namespace jdb
{
  // ___ static __________________________________________________________________________________

  Shader Renderer::m_shader_program_id_ = 0;

  void Renderer::use_shader(const Shader t_id)
  {
    m_shader_program_id_ = t_id;
    glUseProgram(m_shader_program_id_);
  }

  void Renderer::use_texture(const Texture t_id)
  {
    glBindTexture(GL_TEXTURE_2D, t_id);
  }

  void Renderer::draw_mesh(const Mesh_renderer& t_mesh_renderer)
  {
    glBindVertexArray(t_mesh_renderer.id());
  }

  GLuint Renderer::shader()
  {
    return m_shader_program_id_;
  }
}//jdb