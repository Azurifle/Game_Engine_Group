#include <stdafx.h>
#include "Vertex.hpp"

namespace jdb
{
  // ___ constructors ________________________________________________

  Vertex::Vertex(const Vec3<float>& t_pos, const Vec2<float>& t_texture) 
  : pos(t_pos), rgb(Vec3<float>(1)), texture(t_texture) {}
}//jdb