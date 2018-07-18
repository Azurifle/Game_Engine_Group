#ifndef G6037599_MESH_HPP
#define G6037599_MESH_HPP
#pragma once
#include "My_Math/Vec3.hpp"
#include "My_Math/Vec2.hpp"

namespace G6037599
{
  struct Vertex;

  class Mesh final
  {
  public:
    Mesh() = default;
    ~Mesh() = default;
    Mesh(const Mesh& t_to_copy) = default;
    Mesh(Mesh&& t_to_move) noexcept = default;
    Mesh& operator = (const Mesh& t_to_copy) = default;
    Mesh& operator = (Mesh&& t_to_move) noexcept = default;

    void add_vertex(const Vec3<float>& t_pos, const Vec3<float>& t_rgb, const Vec2<float>& t_texture_coord);
    GLsizeiptr memory_size() const;
    std::vector<Vertex> m_vertices_{};
  };
}//G6037599

#endif //G6037599_MESH_HPP
