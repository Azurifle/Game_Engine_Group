#ifndef G6037599_MESH_RENDERER_HPP
#define G6037599_MESH_RENDERER_HPP
#pragma once

namespace G6037599
{
  class Mesh;

  class Mesh_renderer final
  {
  public:
    Mesh_renderer() = default;
    explicit Mesh_renderer(const Mesh& t_mesh);
    ~Mesh_renderer() = default;
    Mesh_renderer(const Mesh_renderer& t_to_copy) = default;
    Mesh_renderer(Mesh_renderer&& t_to_move) noexcept = default;
    Mesh_renderer& operator = (const Mesh_renderer& t_to_copy) = default;
    Mesh_renderer& operator = (Mesh_renderer&& t_to_move) noexcept = default;

    GLuint id() const;
    GLint mvp_location() const;
  private:
    GLuint m_id_{};
    GLint m_mvp_location_{};

    void move_to_vram(const Mesh& t_mesh);
    void get_variables(std::vector<GLint>& t_vram_location);
    static void enable_value_processing(const std::vector<GLint>& t_vram_location);
    static void set_vertex_pointer(const std::vector<GLint>& t_vram_location);
  };
}//G6037599

#endif G6037599_MESH_RENDERER_HPP;
