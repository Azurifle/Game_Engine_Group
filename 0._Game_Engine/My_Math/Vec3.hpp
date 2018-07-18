#ifndef G6037599_VEC3_HPP
#define G6037599_VEC3_HPP
#pragma once

namespace jdb
{
  template <class T>
  class Vec3 final
  {
  public:
    static const Vec3 UP;

    T x, y, z;

    explicit Vec3(T t_x_y = 0);
    ~Vec3() = default;
    Vec3(T t_x, T t_y, T t_z);
    Vec3(const Vec3<T>& t_to_copy) = default;
    Vec3(Vec3&& t_to_move) noexcept = default;
    Vec3& operator = (const Vec3& t_to_copy) = default;
    Vec3& operator = (Vec3&& t_to_move) noexcept = default;

    Vec3 operator += (const Vec3& t_other);
    Vec3 operator -= (const Vec3& t_other);
    Vec3 operator *= (T t_scalar);
    Vec3 operator /= (T t_scalar);

    Vec3 operator + (const Vec3& t_other) const;
    Vec3 operator - (const Vec3& t_other) const;
    Vec3 operator * (T t_scalar) const;
    Vec3 operator / (T t_scalar) const;

    bool operator <= (const Vec3& t_other) const;
    bool operator >= (const Vec3& t_other) const;
    bool operator == (const Vec3& t_other) const;
    bool operator != (const Vec3& t_other) const;
    bool operator < (const Vec3& t_other) const;
    bool operator > (const Vec3& t_other) const;

    std::string to_string() const;
    double squared_size() const;
    double size() const;
    void normalize();

    T dot(const Vec3& t_other) const;
    Vec3 cross(const Vec3& t_other) const;
    T cross(const Vec3& t_v2, const Vec3& t_v3) const;
    Vec3 radian_angles() const;
    double radian_angle_to(const Vec3& t_other) const;
  };
}//jdb

#endif //G6037599_VEC3_HPP