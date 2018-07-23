#include <stdafx.h>
#include "Mesh.hpp"
#include "Vertex.hpp"

namespace jdb
{
  // ___ public ______________________________________________________________________________

  Mesh::Mesh(const GLenum t_draw_mode, const Shader t_shader
    , const std::vector<Vertex>& t_vertices)
  : m_draw_mode_(t_draw_mode), m_shader_(t_shader), m_vertices_count_(t_vertices.size())
  {
    move_to_vram(t_vertices);

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

  GLenum Mesh::draw_mode() const
  {
    return m_draw_mode_;
  }

  Shader Mesh::shader() const
  {
    return m_shader_;
  }

  GLsizei Mesh::vertices_count() const
  {
    return m_vertices_count_;
  }

  GLuint Mesh::id() const
  {
    return m_id_;
  }

  GLint Mesh::vram_mvp() const
  {
    return m_mvp_location_;
  }

  // ___ private ______________________________________________________________________________

  void Mesh::move_to_vram(const std::vector<Vertex>& t_vertices)
  {
    glGenVertexArrays(1, &m_id_);
    glBindVertexArray(m_id_);

    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, t_vertices.size() * sizeof(Vertex), t_vertices.data(), GL_STATIC_DRAW);
  }

  void Mesh::get_variables(std::vector<GLint>& t_vram_locations)
  {
    m_mvp_location_ = glGetUniformLocation(m_shader_, "MVP");
    t_vram_locations.push_back(glGetAttribLocation(m_shader_, "vPos"));
    t_vram_locations.push_back(glGetAttribLocation(m_shader_, "vCol"));
    t_vram_locations.push_back(glGetAttribLocation(m_shader_, "vTex"));
  }

  void Mesh::enable_value_processing(const std::vector<GLint>& t_vram_locations)
  {
    for (auto i : t_vram_locations)
    {
      glEnableVertexAttribArray(i);
    }
  }

  void Mesh::set_vertex_pointer(const GLuint t_attribute, const GLint t_size
    , const void* t_ptr)
  {
    glVertexAttribPointer(t_attribute, t_size, GL_FLOAT, GL_FALSE, sizeof(Vertex), t_ptr);
  }

}//jdb