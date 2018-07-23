#ifndef JDB_VERTEX_HPP
#define JDB_VERTEX_HPP
#pragma once
#include "../my_math/Vec2.hpp"
#include "../my_math/Vec3.hpp"

namespace jdb
{
  struct Vertex final
  {
    Vec3<float> pos{}, rgb{};
    Vec2<float> texture{};

    Vertex(const Vec3<float>& t_pos, const Vec2<float>& t_texture);
  };
}//jdb

#endif //JDB_VERTEX_HPP
