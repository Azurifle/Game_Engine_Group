#include <stdafx.h>
#include "Vec2.hpp"

namespace jdb
{
  //___ (de)constructors/operators ____________________________________
  template <class T>
  Vec2<T>::Vec2(const T t_x_y): x(t_x_y), y(t_x_y) {}

  template <class T>
  Vec2<T>::Vec2(const T t_x, const T t_y): x(t_x), y(t_y) {}

  template <class T>
  Vec2<T> Vec2<T>::operator+=(const Vec2& t_other)
  {
    return { x += t_other.x, y += t_other.y };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator-=(const Vec2& t_other)
  {
    return { x -= t_other.x, y -= t_other.y };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator*=(const T t_scalar)
  {
    return { x *= t_scalar, y *= t_scalar };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator/=(const T t_scalar)
  {
    REQUIRE(t_scalar != 0);
    return { x /= t_scalar, y /= t_scalar };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator+(const Vec2& t_other) const
  {
    return { x + t_other.x, y + t_other.y };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator-(const Vec2& t_other) const
  {
    return { x - t_other.x, y - t_other.y };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator+(T t_scalar) const
  {
    return { x + t_scalar, y + t_scalar };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator-(T t_scalar) const
  {
    return { x - t_scalar, y - t_scalar };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator*(const T t_scalar) const
  {
    return { x * t_scalar, y * t_scalar };
  }

  template <class T>
  Vec2<T> Vec2<T>::operator/(const T t_scalar) const
  {
    REQUIRE(t_scalar != 0);
    return { x / t_scalar, y / t_scalar };
  }

  template <class T>
  bool Vec2<T>::operator<=(const Vec2& t_other) const
  {
    return x <= t_other.x && y <= t_other.y;
  }

  template <class T>
  bool Vec2<T>::operator>=(const Vec2& t_other) const
  {
    return x >= t_other.x && y >= t_other.y;
  }

  template <class T>
  bool Vec2<T>::operator==(const Vec2& t_other) const
  {
    return x == t_other.x && y == t_other.y;
  }

  template <class T>
  bool Vec2<T>::operator!=(const Vec2& t_other) const
  {
    return x != t_other.x && y != t_other.y;
  }

  template <class T>
  bool Vec2<T>::operator<(const Vec2& t_other) const
  {
    return x < t_other.x && y < t_other.y;
  }

  template <class T>
  bool Vec2<T>::operator>(const Vec2& t_other) const
  {
    return x > t_other.x && y > t_other.y;
  }

  //___ public _______________________________________________________
  template <class T>
  std::string Vec2<T>::to_string() const
  {
    return '(' + std::to_string(x) + ',' + std::to_string(y) + ')';
  }

  template <class T>
  double Vec2<T>::squared_size() const
  {
    return x * x + y * y;
  }

  template <class T>
  double Vec2<T>::size() const
  {
    return std::sqrt(squared_size());
  }

  template <class T>
  void Vec2<T>::normalize()
  {
    REQUIRE(size() != 0);
    *this /= static_cast<T>(round(size()));
  }

  template <class T>
  T Vec2<T>::dot(const Vec2& t_other) const
  {
    return x*t_other.x + y*t_other.y;
  }

  template <class T>
  T Vec2<T>::cross(const Vec2& t_other) const
  {
    return x * t_other.y - y * t_other.x;
  }

  template <class T>
  Vec2<T> Vec2<T>::get_det() const
  {
    return {y, -x};
  }

  template <class T>
  double Vec2<T>::radian_angle() const
  {
    REQUIRE(x != 0);
    return atan(y/x);
  }

  template <class T>
  double Vec2<T>::radian_angle_to(const Vec2& t_other) const
  {
    REQUIRE(size() != 0); REQUIRE(t_other.size() != 0);
    return acos(dot(t_other) / (size() * t_other.size()));
  }

  template class Vec2<int>;
  template class Vec2<float>;
  template class Vec2<double>;
}//jdb