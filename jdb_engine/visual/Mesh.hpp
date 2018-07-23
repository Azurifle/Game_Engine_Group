#ifndef JDB_MESH_RENDERER_HPP
#define JDB_MESH_RENDERER_HPP
#pragma once
#include "Shader_manager.hpp"

namespace jdb
{
  struct Vertex;

  class Mesh final
  {
  public:
    Mesh() = default;
    Mesh(GLenum t_draw_mode, Shader t_shader, const std::vector<Vertex>& t_vertices);
    ~Mesh() = default;
    Mesh(const Mesh& t_to_copy) = default;
    Mesh(Mesh&& t_to_move) noexcept = default;
    Mesh& operator = (const Mesh& t_to_copy) = default;
    Mesh& operator = (Mesh&& t_to_move) noexcept = default;

    GLenum draw_mode() const;
    Shader shader() const;
    GLsizei vertices_count() const;
    GLuint id() const;
    GLint vram_mvp() const;
  private:
    GLenum m_draw_mode_{};
    Shader m_shader_{};
    GLsizei m_vertices_count_{};
    GLuint m_id_{};
    GLint m_mvp_location_{};

    void move_to_vram(const std::vector<Vertex>& t_vertices);
    void get_variables(std::vector<GLint>& t_vram_location);
    static void enable_value_processing(const std::vector<GLint>& t_vram_location);
    static void set_vertex_pointer(GLuint t_attribute, GLint t_size, const void* t_ptr = nullptr);
  };
}//jdb

#endif JDB_MESH_RENDERER_HPP;
