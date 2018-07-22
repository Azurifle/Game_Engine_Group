#include <stdafx.h>
#include "Mesh.hpp"
#include "Vertex.hpp"

namespace jdb
{
  // ___ public __________________________________________________________________________________

  void Mesh::add_vertex(const Vec3<float>& t_pos, const Vec3<float>& t_rgb, const Vec2<float>& t_texture_xy)
  {
    vertices.push_back(Vertex(t_pos, t_rgb, t_texture_xy));
  }

  GLsizeiptr Mesh::memory_size() const
  {
    return vertices.size() * sizeof(Vertex);
  }
}//jdb