#include <stdafx.h>
#include "Mesh.hpp"
#include "Mesh_renderer.hpp"
#include "Renderer.hpp"
#include "Vertex.hpp"

namespace jdb
{
  // ___ public ______________________________________________________________________________

  Mesh_renderer::Mesh_renderer(const Mesh& t_mesh, const GLenum t_draw_mode)
  : m_mode_(t_draw_mode), m_vertices_count_(t_mesh.vertices.size())
  {
    move_to_vram(t_mesh);

    std::vector<GLint> vram_locations;
    get_variables(vram_locations);
    enable_value_processing(vram_locations);

    enum Enum
    {
      POS, COLOR, TEXTURE
      , TEXTURE_XY = 2, XYZ_RGB, XYZ_RGB_SIZE = sizeof(float) * XYZ_RGB
    };
    set_vertex_pointer(vram_locations[POS], XYZ_RGB);
    set_vertex_pointer(vram_locations[COLOR], XYZ_RGB, reinterpret_cast<void*>(XYZ_RGB_SIZE));
    static const auto TEXTURE_PTR = reinterpret_cast<void*>(XYZ_RGB_SIZE * TEXTURE_XY);
    set_vertex_pointer(vram_locations[TEXTURE], TEXTURE_XY, TEXTURE_PTR);
  }

  GLint Mesh_renderer::mvp_location() const
  {
    return m_mvp_location_;
  }

  GLuint Mesh_renderer::id() const
  {
    return m_id_;
  }

  GLenum Mesh_renderer::mode() const
  {
    return m_mode_;
  }

  GLsizei Mesh_renderer::vertices_count() const
  {
    return m_vertices_count_;
  }

  // ___ private ______________________________________________________________________________

  void Mesh_renderer::move_to_vram(const Mesh& t_mesh)
  {
    glGenVertexArrays(1, &m_id_);
    glBindVertexArray(m_id_);

    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, t_mesh.memory_size(), t_mesh.vertices.data(), GL_STATIC_DRAW);
  }

  void Mesh_renderer::get_variables(std::vector<GLint>& t_vram_locations)
  {
    m_mvp_location_ = glGetUniformLocation(Renderer::shader(), "MVP");
    t_vram_locations.push_back(glGetAttribLocation(Renderer::shader(), "vPos"));
    t_vram_locations.push_back(glGetAttribLocation(Renderer::shader(), "vCol"));
    t_vram_locations.push_back(glGetAttribLocation(Renderer::shader(), "vTex"));
  }

  void Mesh_renderer::enable_value_processing(const std::vector<GLint>& t_vram_locations)
  {
    for (auto i : t_vram_locations)
    {
      glEnableVertexAttribArray(i);
    }
  }

  void Mesh_renderer::set_vertex_pointer(const GLuint t_attribute, const GLint t_size
    , const void* t_ptr)
  {
    glVertexAttribPointer(t_attribute, t_size, GL_FLOAT, GL_FALSE, sizeof(Vertex), t_ptr);
  }

}//jdb