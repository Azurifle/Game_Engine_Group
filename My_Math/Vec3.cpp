#include <stdafx.h>
#include "Vec3.hpp"

namespace G6037599
{
  //___ static ____________________________________
  template <class T>
  const Vec3<T> Vec3<T>::UP(0, 1, 0);

  //___ (de)constructors/operators ____________________________________
  template <class T>
  Vec3<T>::Vec3(const T t_x_y) : x(t_x_y), y(t_x_y), z(t_x_y) {}

  template <class T>
  Vec3<T>::Vec3(const T t_x, const T t_y, const T t_z) 
    : x(t_x), y(t_y), z(t_z) {}

  template <class T>
  Vec3<T> Vec3<T>::operator+=(const Vec3<T>& t_other)
  {
    return { x += t_other.x, y += t_other.y, z += t_other.z };
  }

  template <class T>
  Vec3<T> Vec3<T>::operator-=(const Vec3<T>& t_other)
  {
    return { x -= t_other.x, y -= t_other.y, z -= t_other.z };
  }

  template <class T>
  Vec3<T> Vec3<T>::operator*=(const T t_scalar)
  {
    return { x *= t_scalar, y *= t_scalar, z *= t_scalar };
  }

  template <class T>
  Vec3<T> Vec3<T>::operator/=(const T t_scalar)
  {
    REQUIRE(t_scalar != 0);
    return { x /= t_scalar, y /= t_scalar, z /= t_scalar };
  }

  template <class T>
  Vec3<T> Vec3<T>::operator+(const Vec3<T>& t_other) const
  {
    return { x + t_other.x, y + t_other.y, z + t_other.z };
  }

  template <class T>
  Vec3<T> Vec3<T>::operator-(const Vec3<T>& t_other) const
  {
    return { x - t_other.x, y - t_other.y, z - t_other.z };
  }

  template <class T>
  Vec3<T> Vec3<T>::operator*(const T t_scalar) const
  {
    return { x * t_scalar, y * t_scalar, z * t_scalar };
  }

  template <class T>
  Vec3<T> Vec3<T>::operator/(const T t_scalar) const
  {
    REQUIRE(t_scalar != 0);
    return { x / t_scalar, y / t_scalar, z / t_scalar };
  }

  template <class T>
  bool Vec3<T>::operator<=(const Vec3<T>& t_other) const
  {
    return x <= t_other.x && y <= t_other.y && z <= t_other.z;
  }

  template <class T>
  bool Vec3<T>::operator>=(const Vec3<T>& t_other) const
  {
    return x >= t_other.x && y >= t_other.y && z >= t_other.z;
  }

  template <class T>
  bool Vec3<T>::operator==(const Vec3<T>& t_other) const
  {
    return x == t_other.x && y == t_other.y && z == t_other.z;
  }

  template <class T>
  bool Vec3<T>::operator!=(const Vec3<T>& t_other) const
  {
    return x != t_other.x && y != t_other.y && z != t_other.z;
  }

  template <class T>
  bool Vec3<T>::operator<(const Vec3<T>& t_other) const
  {
    return x < t_other.x && y < t_other.y && z < t_other.z;
  }

  template <class T>
  bool Vec3<T>::operator>(const Vec3<T>& t_other) const
  {
    return x > t_other.x && y > t_other.y && z > t_other.z;
  }

  //___ public _______________________________________________________
  template <class T>
  std::string Vec3<T>::to_string() const
  {
    return '(' + std::to_string(x) + ',' + std::to_string(y) + ',' 
      + std::to_string(z) + ')';
  }

  template <class T>
  T Vec3<T>::dot(const Vec3<T>& t_other) const
  {
    return x * t_other.x + y * t_other.y + z * t_other.z;
  }

  template <class T>
  Vec3<T> Vec3<T>::cross(const Vec3<T>& t_other) const
  {
    return { y * t_other.z - z * t_other.y, z * t_other.x - x * t_other.z
      , x * t_other.y - y * t_other.x };
  }

  template <class T>
  T Vec3<T>::cross(const Vec3<T>& t_v2, const Vec3<T>& t_v3) const
  {
    return cross(t_v2).dot(t_v3);
  }

  template <class T>
  double Vec3<T>::squared_size() const
  {
    return x * x + y * y + z * z;
  }

  template <class T>
  double Vec3<T>::size() const
  {
    return std::sqrt(squared_size());
  }

  template <class T>
  void Vec3<T>::normalize()
  {
    REQUIRE(size() != 0);
    *this /= static_cast<T>(round(size()));
  }

  template <class T>
  Vec3<T> Vec3<T>::radian_angles() const
  {
    REQUIRE(x != 0); 
    REQUIRE(size() != 0);

    const double THETA = atan(y / x), P = acos(z / size())
      , ANGLE_X = round(size()*sin(P)*cos(THETA))
      , ANGLE_Y = round(size()*sin(P)*sin(THETA))
      , ANGLE_Z = round(size()*cos(P));
    
    return { static_cast<T>(ANGLE_X), static_cast<T>(ANGLE_Y)
      , static_cast<T>(ANGLE_Z) };
  }

  template <class T>
  double Vec3<T>::radian_angle_to(const Vec3<T>& t_other) const
  {
    REQUIRE(size() != 0); 
    REQUIRE(t_other.size() != 0);

    return acos(dot(t_other) / (size() * t_other.size()));
  }

  template class Vec3<int>;
  template class Vec3<float>;
  template class Vec3<double>;
}//G6037599