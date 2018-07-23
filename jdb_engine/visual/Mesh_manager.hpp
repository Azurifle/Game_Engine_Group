#ifndef JDB_MESH_HPP
#define JDB_MESH_HPP
#pragma once
#include "../my_math/Vec3.hpp"
#include "../my_math/Vec2.hpp"
#include "Shader_manager.hpp"

namespace jdb
{
  class Mesh;
  struct Vertex;

  class Mesh_manager final
  {
  public:
    static Mesh_manager instance;

    static void new_mesh(GLenum t_draw_mode, Shader t_shader);

    //t_texture_xy: x = {left:0.0f - right:1.0f}, y = {top:0.0f - bottom:1.0f}
    //Add vertex clock wise
    static void add_vertex(const Vec3<float>& t_pos, const Vec2<float>& t_texture_xy);
    static Mesh&& save_mesh();
  private:
    std::vector<Vertex> m_vertices_{};
    GLenum m_draw_mode_{};
    Shader m_shader_{};

    Mesh_manager() = default;
    ~Mesh_manager() = default;
    Mesh_manager(const Mesh_manager& t_to_copy) = default;
    Mesh_manager(Mesh_manager&& t_to_move) noexcept = default;
    Mesh_manager& operator = (const Mesh_manager& t_to_copy) = default;
    Mesh_manager& operator = (Mesh_manager&& t_to_move) noexcept = default;
  };
}//jdb

#endif //JDB_MESH_HPP