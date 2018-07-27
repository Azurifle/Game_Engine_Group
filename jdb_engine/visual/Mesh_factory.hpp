#ifndef JDB_MESH_HPP
#define JDB_MESH_HPP
#pragma once
#include "Shader_manager.hpp"
#include "../my_math/Vec3.hpp"
#include "../my_math/Vec2.hpp"
#include "Vertex.hpp"

namespace jdb
{
  class Mesh;

  class Mesh_factory final
  {
  public:
    explicit Mesh_factory(GLenum t_draw_mode, Shader t_shader = Shader_manager::load_or_get());
    ~Mesh_factory() = default;
    Mesh_factory(const Mesh_factory& t_to_copy) = default;
    Mesh_factory(Mesh_factory&& t_to_move) noexcept = default;
    Mesh_factory& operator = (const Mesh_factory& t_to_copy) = default;
    Mesh_factory& operator = (Mesh_factory&& t_to_move) noexcept = default;

    void new_mesh(GLenum t_draw_mode, Shader t_shader = Shader_manager::load_or_get());

    //t_texture_xy: x = {left:0.0f - right:1.0f}, y = {top:0.0f - bottom:1.0f}
    //Add vertex clock wise
    void add_vertex(const Vec3<float>& t_pos, const Vec2<float>& t_texture_xy);
    std::unique_ptr<Mesh> save_mesh();
  private:
    std::vector<Vertex> m_vertices_{};
    GLenum m_draw_mode_{};
    Shader m_shader_{};
  };
}//jdb

#endif //JDB_MESH_HPP