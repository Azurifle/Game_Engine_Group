#ifndef JDB_MESH_RENDERER_HPP
#define JDB_MESH_RENDERER_HPP
#pragma once

namespace jdb
{
  class Mesh;

  class Mesh_renderer final
  {
  public:
    Mesh_renderer() = default;
    Mesh_renderer(const Mesh& t_mesh, GLenum t_draw_mode);
    ~Mesh_renderer() = default;
    Mesh_renderer(const Mesh_renderer& t_to_copy) = default;
    Mesh_renderer(Mesh_renderer&& t_to_move) noexcept = default;
    Mesh_renderer& operator = (const Mesh_renderer& t_to_copy) = default;
    Mesh_renderer& operator = (Mesh_renderer&& t_to_move) noexcept = default;

    GLint mvp_location() const;
    GLuint id() const;
    GLenum mode() const;
    GLsizei vertices_count() const;
  private:
    GLint m_mvp_location_{};
    GLuint m_id_{};
    GLenum m_mode_{};
    GLsizei m_vertices_count_{};

    void move_to_vram(const Mesh& t_mesh);
    void get_variables(std::vector<GLint>& t_vram_location);
    static void enable_value_processing(const std::vector<GLint>& t_vram_location);
    static void set_vertex_pointer(GLuint t_attribute, GLint t_size, const void* t_ptr = nullptr);
  };
}//jdb

#endif JDB_MESH_RENDERER_HPP;
