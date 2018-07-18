#ifndef G6037599_TRIANGLE_HPP
#define G6037599_TRIANGLE_HPP
#pragma once
#include "../My_Math/Vec3.hpp"

namespace G6037599
{
  class Triangle final
  {
  public:
    Vec3<float> pos{}, scale{};
    float z_degree, rotate_speed;

    explicit Triangle(const Vec3<float>& t_pos = Vec3<float>(0)
      , const Vec3<float>& t_scale = Vec3<float>(1)
      , float t_z_degree = 0, float t_rotate_speed = 180);
    ~Triangle() = default;
    Triangle(const Triangle& t_to_copy) = default;
    Triangle(Triangle&& t_to_move) noexcept = default;
    Triangle& operator = (const Triangle& t_to_copy) = default;
    Triangle& operator = (Triangle&& t_to_move) noexcept = default;

    void update(float t_delta_seconds);
  private:
    float m_x_degree_;

    void draw() const;
  };
}//G6037599

#endif //G6037599_TRIANGLE_HPP