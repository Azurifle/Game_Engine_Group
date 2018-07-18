#include <stdafx.h>
#include "Mesh.hpp"
#include "Vertex.hpp"

namespace jdb
{
  // ___ public __________________________________________________________________________________

  void Mesh::add_vertex(const Vec3<float>& t_pos, const Vec3<float>& t_rgb, const Vec2<float>& t_texture_coord)
  {
    m_vertices_.push_back(Vertex(t_pos, t_rgb, t_texture_coord));
  }

  GLsizeiptr Mesh::memory_size() const
  {
    return m_vertices_.size() * sizeof(Vertex);
  }
}//jdb