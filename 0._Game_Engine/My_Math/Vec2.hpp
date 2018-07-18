#ifndef G6037599_VEC2_HPP
#define G6037599_VEC2_HPP
#pragma once

namespace jdb
{
  template <class T>
  class Vec2 final
  {
  public:
    T x, y;//if there is no setter constraint make it public

    explicit Vec2(T t_x_y = 0);
    ~Vec2() = default;
    Vec2(T t_x, T t_y);
    Vec2(const Vec2& t_to_copy) = default;
    Vec2(Vec2&& t_to_move) noexcept = default;
    Vec2& operator = (const Vec2& t_to_copy) = default;
    Vec2& operator = (Vec2&& t_to_move) noexcept = default;

    Vec2 operator += (const Vec2& t_other);
    Vec2 operator -= (const Vec2& t_other);
    Vec2 operator *= (T t_scalar);
    Vec2 operator /= (T t_scalar);

    Vec2 operator + (const Vec2& t_other) const;
    Vec2 operator - (const Vec2& t_other) const;
    Vec2 operator * (T t_scalar) const;
    Vec2 operator / (T t_scalar) const;

    bool operator <= (const Vec2& t_other) const;
    bool operator >= (const Vec2& t_other) const;
    bool operator == (const Vec2& t_other) const;
    bool operator != (const Vec2& t_other) const;
    bool operator < (const Vec2& t_other) const;
    bool operator > (const Vec2& t_other) const;

    std::string to_string() const;
    double squared_size() const;//Faster than size() (no sqrt())
    double size() const;
    void normalize();

    T dot(const Vec2& t_other) const;
    T cross(const Vec2& t_other) const;
    Vec2 get_det() const;
    double radian_angle() const;
    double radian_angle_to(const Vec2& t_other) const;
  };
}//jdb

#endif //G6037599_VEC2_HPP