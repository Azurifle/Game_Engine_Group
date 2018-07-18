#ifndef G6037599_VEC4_HPP
#define G6037599_VEC4_HPP
#pragma once

namespace G6037599
{
  template <class T>
  class Vec4 final
  {
  public:
    static const Vec4 UP;

    T x, y, z, t;

    explicit Vec4(T t_x_y = 0);
    ~Vec4() = default;
    Vec4(T t_x, T t_y, T t_z, T t_t);
    Vec4(const Vec4& t_to_copy) = default;
    Vec4(Vec4&& t_to_move) noexcept = default;
    Vec4& operator = (const Vec4& t_to_copy) = default;
    Vec4& operator = (Vec4&& t_to_move) noexcept = default;

    Vec4 operator += (const Vec4& t_other);
    Vec4 operator -= (const Vec4& t_other);
    Vec4 operator *= (T t_scalar);
    Vec4 operator /= (T t_scalar);

    Vec4 operator + (const Vec4& t_other) const;
    Vec4 operator - (const Vec4& t_other) const;
    Vec4 operator * (T t_scalar) const;
    Vec4 operator / (T t_scalar) const;

    bool operator <= (const Vec4& t_other) const;
    bool operator >= (const Vec4& t_other) const;
    bool operator == (const Vec4& t_other) const;
    bool operator != (const Vec4& t_other) const;
    bool operator < (const Vec4& t_other) const;
    bool operator > (const Vec4& t_other) const;

    std::string to_string() const;
    double squared_size() const;
    double size() const;
    void normalize();

    T dot(const Vec4& t_other) const;
    double radian_angle_to(const Vec4& t_other) const;
  };
}//G6037599

#endif //G6037599_VEC4_HPP