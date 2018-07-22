#include <stdafx.h>
#include "Vec4.hpp"

namespace jdb
{
  //___ static ____________________________________
  template <class T>
  const Vec4<T> Vec4<T>::UP(0, 1, 0, 0);

  //___ (de)constructors/operators ____________________________________
  template <class T>
  Vec4<T>::Vec4(const T t_x_y) : x(t_x_y), y(t_x_y), z(t_x_y), t(t_x_y) {}

  template <class T>
  Vec4<T>::Vec4(const T t_x, const T t_y, const T t_z, const T t_t)
    : x(t_x), y(t_y), z(t_z), t(t_t) {}

  template <class T>
  Vec4<T> Vec4<T>::operator+=(const Vec4<T>& t_other)
  {
    return { x += t_other.x, y += t_other.y, z += t_other.z, t += t_other.t };
  }

  template <class T>
  Vec4<T> Vec4<T>::operator-=(const Vec4<T>& t_other)
  {
    return { x -= t_other.x, y -= t_other.y, z -= t_other.z, t -= t_other.t };
  }

  template <class T>
  Vec4<T> Vec4<T>::operator*=(const T t_scalar)
  {
    return { x *= t_scalar, y *= t_scalar, z *= t_scalar, t *= t_scalar };
  }

  template <class T>
  Vec4<T> Vec4<T>::operator/=(const T t_scalar)
  {
    REQUIRE(t_scalar != 0);
    return { x /= t_scalar, y /= t_scalar, z /= t_scalar, t /= t_scalar };
  }

  template <class T>
  Vec4<T> Vec4<T>::operator+(const Vec4<T>& t_other) const
  {
    return { x + t_other.x, y + t_other.y, z + t_other.z, t + t_other.t };
  }

  template <class T>
  Vec4<T> Vec4<T>::operator-(const Vec4<T>& t_other) const
  {
    return { x - t_other.x, y - t_other.y, z - t_other.z, t - t_other.t };
  }

  template <class T>
  Vec4<T> Vec4<T>::operator*(const T t_scalar) const
  {
    return { x * t_scalar, y * t_scalar, z * t_scalar, t * t_scalar };
  }

  template <class T>
  Vec4<T> Vec4<T>::operator/(const T t_scalar) const
  {
    REQUIRE(t_scalar != 0);
    return { x / t_scalar, y / t_scalar, z / t_scalar, t / t_scalar };
  }

  template <class T>
  bool Vec4<T>::operator<=(const Vec4<T>& t_other) const
  {
    return x <= t_other.x && y <= t_other.y && z <= t_other.z;
  }

  template <class T>
  bool Vec4<T>::operator>=(const Vec4<T>& t_other) const
  {
    return x >= t_other.x && y >= t_other.y && z >= t_other.z 
      && t >= t_other.t;
  }

  template <class T>
  bool Vec4<T>::operator==(const Vec4<T>& t_other) const
  {
    return x == t_other.x && y == t_other.y && z == t_other.z
      && t == t_other.t;
  }

  template <class T>
  bool Vec4<T>::operator!=(const Vec4<T>& t_other) const
  {
    return x != t_other.x && y != t_other.y && z != t_other.z
      && t != t_other.t;
  }

  template <class T>
  bool Vec4<T>::operator<(const Vec4<T>& t_other) const
  {
    return x < t_other.x && y < t_other.y && z < t_other.z
      && t < t_other.t;
  }

  template <class T>
  bool Vec4<T>::operator>(const Vec4<T>& t_other) const
  {
    return x > t_other.x && y > t_other.y && z > t_other.z
      && t > t_other.t;
  }

  //___ public _______________________________________________________
  template <class T>
  std::string Vec4<T>::to_string() const
  {
    return '(' + std::to_string(x) + ',' + std::to_string(y) + ',' 
      + std::to_string(z) + ',' + std::to_string(t) + ')';
  }

  template <class T>
  T Vec4<T>::dot(const Vec4<T>& t_other) const
  {
    return x * t_other.x + y * t_other.y + z * t_other.z + t * t_other.t;
  }

  template <class T>
  double Vec4<T>::squared_size() const
  {
    return x * x + y * y + z * z + t * t;
  }

  template <class T>
  double Vec4<T>::size() const
  {
    return std::sqrt(squared_size());
  }

  template <class T>
  void Vec4<T>::normalize()
  {
    REQUIRE(size() != 0);
    *this /= static_cast<T>(round(size()));
  }

  template <class T>
  double Vec4<T>::radian_angle_to(const Vec4<T>& t_other) const
  {
    REQUIRE(size() != 0); 
    REQUIRE(t_other.size() != 0);

    return acos(dot(t_other) / (size() * t_other.size()));
  }

  template class Vec4<int>;
  Vec4<int> operator+(const Vec4<int>& t_left, const Vec3<int>& t_right)
  {
    return { t_left.x + t_right.x, t_left.y + t_right.y, t_left.z + t_right.z, t_left.t };
  }

  template class Vec4<float>;
  Vec4<float> operator+(const Vec4<float>& t_left, const Vec3<float>& t_right)
  {
    return { t_left.x + t_right.x, t_left.y + t_right.y, t_left.z + t_right.z, t_left.t };
  }

  template class Vec4<double>;
  Vec4<double> operator+(const Vec4<double>& t_left, const Vec3<double>& t_right)
  {
    return { t_left.x + t_right.x, t_left.y + t_right.y, t_left.z + t_right.z, t_left.t };
  } 

}//jdb