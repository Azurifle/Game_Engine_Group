#ifndef G6037599_VERTEX_HPP
#define G6037599_VERTEX_HPP
#pragma once
#include "../My_Math/Vec3.hpp"
#include "../My_Math/Vec2.hpp"

namespace G6037599
{
  struct Vertex final
  {
    Vec3<float> pos{}, rgb{};
    Vec2<float> texture{};

    Vertex(const Vec3<float>& t_pos, const Vec3<float>& t_rgb, const Vec2<float>& t_texture = Vec2<float>(0));
  };
}//G6037599

#endif //G6037599_VERTEX_HPP
