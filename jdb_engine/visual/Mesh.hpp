#ifndef JDB_MESH_HPP
#define JDB_MESH_HPP
#pragma once
#include "../my_math/Vec3.hpp"
#include "../my_math/Vec2.hpp"

namespace jdb
{
  struct Vertex;

  class Mesh final
  {
  public:
    std::vector<Vertex> vertices{};

    Mesh() = default;
    ~Mesh() = default;
    Mesh(const Mesh& t_to_copy) = default;
    Mesh(Mesh&& t_to_move) noexcept = default;
    Mesh& operator = (const Mesh& t_to_copy) = default;
    Mesh& operator = (Mesh&& t_to_move) noexcept = default;

    //t_texture_xy: x = {left:0.0f - right:1.0f}, y = {top:0.0f - bottom:1.0f}
    //Add vertex clock wise
    void add_vertex(const Vec3<float>& t_pos, const Vec2<float>& t_texture_xy);
    GLsizeiptr memory_size() const;
  };
}//jdb

#endif //JDB_MESH_HPP
