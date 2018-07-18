#include <stdafx.h>
#include "Mat4.hpp"
#include "../Game_engine.hpp"

namespace jdb
{
  // ___ static _________________________________________________________________
  Mat4 Mat4::identity()
  {
    return Mat4(1);
  }

  Mat4 Mat4::translation(const Vec3<float>& t_translate)
  {
    Mat4 temp(1);
    temp.m_mat_[0][T] = t_translate.x;
    temp.m_mat_[1][T] = t_translate.y;
    temp.m_mat_[2][T] = t_translate.z;
    return temp;
  }

  Mat4 Mat4::rotation(const float t_radian_angle, const Vec3<int>& t_axis)
  {
    //adapt from http://www.songho.ca/opengl/gl_matrix.html

    Mat4 rot(1);
    const auto C = cos(t_radian_angle), S = sin(t_radian_angle)
      , ONE_MINUS_C = 1 - C
      , XY_1_MINUS_C = t_axis.x*t_axis.y*ONE_MINUS_C
      , XZ_1_MINUS_C = t_axis.x*t_axis.z*ONE_MINUS_C
      , YZ_1_MINUS_C = t_axis.y*t_axis.z*ONE_MINUS_C
      , Z_S = t_axis.z*S, Y_S = t_axis.y*S, X_S = t_axis.x*S;

    rot.m_mat_[0][X] = t_axis.x*t_axis.x*ONE_MINUS_C + C;
    rot.set(Vec2<int>(Y, 0), XY_1_MINUS_C - Z_S);
    rot.m_mat_[0][Z] = XZ_1_MINUS_C + Y_S;

    rot.m_mat_[1][X] = XY_1_MINUS_C + Z_S;
    rot.m_mat_[1][Y] = t_axis.y*t_axis.y*ONE_MINUS_C + C;
    rot.m_mat_[1][Z] = YZ_1_MINUS_C - X_S;

    rot.m_mat_[2][X] = XZ_1_MINUS_C - Y_S;
    rot.m_mat_[2][Y] = YZ_1_MINUS_C + X_S;
    rot.m_mat_[2][Z] = t_axis.z*t_axis.z*ONE_MINUS_C + C;

    return rot;
  }

  Mat4 Mat4::scaling(const Vec3<float>& t_scale)
  {
    Mat4 temp(1);
    temp.m_mat_[0][X] = t_scale.x;
    temp.m_mat_[1][Y] = t_scale.y;
    temp.m_mat_[2][Z] = t_scale.z;
    return temp;
  }

  Mat4 Mat4::transpose(const Mat4& t_matrix)
  {
    Mat4 temp;
    for (auto row = 0; row < SIZE; ++row)
    {
      for (auto col = 0; col < SIZE; ++col)
      {
        temp.set(Vec2<int>(col, row), t_matrix.m_mat_[col][row]);
      }
    }
    return temp;
  }

  Mat4 Mat4::inverse(const Mat4& t_m)
  {
    //adapt from https://d.pr/n/h8W2Mk

    Mat4 inv;
    inv.m_mat_[0][X] = t_m.m_mat_[1][Y] * t_m.m_mat_[2][Z] * t_m.m_mat_[3][T] -
      t_m.m_mat_[1][Y] * t_m.m_mat_[2][T] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[2][Y] * t_m.m_mat_[1][Z] * t_m.m_mat_[3][T] +
      t_m.m_mat_[2][Y] * t_m.m_mat_[1][T] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[3][Y] * t_m.m_mat_[1][Z] * t_m.m_mat_[2][T] -
      t_m.m_mat_[3][Y] * t_m.m_mat_[1][T] * t_m.m_mat_[2][Z];

    inv.m_mat_[1][X] = -t_m.m_mat_[1][X] * t_m.m_mat_[2][Z] * t_m.m_mat_[3][T] +
      t_m.m_mat_[1][X] * t_m.m_mat_[2][T] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[2][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[3][T] -
      t_m.m_mat_[2][X] * t_m.m_mat_[1][T] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[3][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[2][T] +
      t_m.m_mat_[3][X] * t_m.m_mat_[1][T] * t_m.m_mat_[2][Z];

    inv.m_mat_[2][X] = t_m.m_mat_[1][X] * t_m.m_mat_[2][Y] * t_m.m_mat_[3][T] -
      t_m.m_mat_[1][X] * t_m.m_mat_[2][T] * t_m.m_mat_[3][Y] -
      t_m.m_mat_[2][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[3][T] +
      t_m.m_mat_[2][X] * t_m.m_mat_[1][T] * t_m.m_mat_[3][Y] +
      t_m.m_mat_[3][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[2][T] -
      t_m.m_mat_[3][X] * t_m.m_mat_[1][T] * t_m.m_mat_[2][Y];

    inv.m_mat_[3][X] = -t_m.m_mat_[1][X] * t_m.m_mat_[2][Y] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[1][X] * t_m.m_mat_[2][Z] * t_m.m_mat_[3][Y] +
      t_m.m_mat_[2][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[2][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[3][Y] -
      t_m.m_mat_[3][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[2][Z] +
      t_m.m_mat_[3][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[2][Y];

    inv.m_mat_[0][Y] = -t_m.m_mat_[0][Y] * t_m.m_mat_[2][Z] * t_m.m_mat_[3][T] +
      t_m.m_mat_[0][Y] * t_m.m_mat_[2][T] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[2][Y] * t_m.m_mat_[0][Z] * t_m.m_mat_[3][T] -
      t_m.m_mat_[2][Y] * t_m.m_mat_[0][T] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[3][Y] * t_m.m_mat_[0][Z] * t_m.m_mat_[2][T] +
      t_m.m_mat_[3][Y] * t_m.m_mat_[0][T] * t_m.m_mat_[2][Z];

    inv.m_mat_[1][Y] = t_m.m_mat_[0][X] * t_m.m_mat_[2][Z] * t_m.m_mat_[3][T] -
      t_m.m_mat_[0][X] * t_m.m_mat_[2][T] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[3][T] +
      t_m.m_mat_[2][X] * t_m.m_mat_[0][T] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[2][T] -
      t_m.m_mat_[3][X] * t_m.m_mat_[0][T] * t_m.m_mat_[2][Z];

    inv.m_mat_[2][Y] = -t_m.m_mat_[0][X] * t_m.m_mat_[2][Y] * t_m.m_mat_[3][T] +
      t_m.m_mat_[0][X] * t_m.m_mat_[2][T] * t_m.m_mat_[3][Y] +
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[3][T] -
      t_m.m_mat_[2][X] * t_m.m_mat_[0][T] * t_m.m_mat_[3][Y] -
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[2][T] +
      t_m.m_mat_[3][X] * t_m.m_mat_[0][T] * t_m.m_mat_[2][Y];

    inv.m_mat_[3][Y] = t_m.m_mat_[0][X] * t_m.m_mat_[2][Y] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[0][X] * t_m.m_mat_[2][Z] * t_m.m_mat_[3][Y] -
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[3][Y] +
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[2][Z] -
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[2][Y];

    inv.m_mat_[0][Z] = t_m.m_mat_[0][Y] * t_m.m_mat_[1][Z] * t_m.m_mat_[3][T] -
      t_m.m_mat_[0][Y] * t_m.m_mat_[1][T] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[1][Y] * t_m.m_mat_[0][Z] * t_m.m_mat_[3][T] +
      t_m.m_mat_[1][Y] * t_m.m_mat_[0][T] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[3][Y] * t_m.m_mat_[0][Z] * t_m.m_mat_[1][T] -
      t_m.m_mat_[3][Y] * t_m.m_mat_[0][T] * t_m.m_mat_[1][Z];

    inv.m_mat_[1][Z] = -t_m.m_mat_[0][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[3][T] +
      t_m.m_mat_[0][X] * t_m.m_mat_[1][T] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[3][T] -
      t_m.m_mat_[1][X] * t_m.m_mat_[0][T] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[1][T] +
      t_m.m_mat_[3][X] * t_m.m_mat_[0][T] * t_m.m_mat_[1][Z];

    inv.m_mat_[2][Z] = t_m.m_mat_[0][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[3][T] -
      t_m.m_mat_[0][X] * t_m.m_mat_[1][T] * t_m.m_mat_[3][Y] -
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[3][T] +
      t_m.m_mat_[1][X] * t_m.m_mat_[0][T] * t_m.m_mat_[3][Y] +
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[1][T] -
      t_m.m_mat_[3][X] * t_m.m_mat_[0][T] * t_m.m_mat_[1][Y];

    inv.m_mat_[3][Z] = -t_m.m_mat_[0][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[3][Z] +
      t_m.m_mat_[0][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[3][Y] +
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[3][Z] -
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[3][Y] -
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[1][Z] +
      t_m.m_mat_[3][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[1][Y];

    inv.m_mat_[0][T] = -t_m.m_mat_[0][Y] * t_m.m_mat_[1][Z] * t_m.m_mat_[2][T] +
      t_m.m_mat_[0][Y] * t_m.m_mat_[1][T] * t_m.m_mat_[2][Z] +
      t_m.m_mat_[1][Y] * t_m.m_mat_[0][Z] * t_m.m_mat_[2][T] -
      t_m.m_mat_[1][Y] * t_m.m_mat_[0][T] * t_m.m_mat_[2][Z] -
      t_m.m_mat_[2][Y] * t_m.m_mat_[0][Z] * t_m.m_mat_[1][T] +
      t_m.m_mat_[2][Y] * t_m.m_mat_[0][T] * t_m.m_mat_[1][Z];

    inv.m_mat_[1][T] = t_m.m_mat_[0][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[2][T] -
      t_m.m_mat_[0][X] * t_m.m_mat_[1][T] * t_m.m_mat_[2][Z] -
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[2][T] +
      t_m.m_mat_[1][X] * t_m.m_mat_[0][T] * t_m.m_mat_[2][Z] +
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[1][T] -
      t_m.m_mat_[2][X] * t_m.m_mat_[0][T] * t_m.m_mat_[1][Z];

    inv.m_mat_[2][T] = -t_m.m_mat_[0][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[2][T] +
      t_m.m_mat_[0][X] * t_m.m_mat_[1][T] * t_m.m_mat_[2][Y] +
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[2][T] -
      t_m.m_mat_[1][X] * t_m.m_mat_[0][T] * t_m.m_mat_[2][Y] -
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[1][T] +
      t_m.m_mat_[2][X] * t_m.m_mat_[0][T] * t_m.m_mat_[1][Y];

    inv.m_mat_[3][T] = t_m.m_mat_[0][X] * t_m.m_mat_[1][Y] * t_m.m_mat_[2][Z] -
      t_m.m_mat_[0][X] * t_m.m_mat_[1][Z] * t_m.m_mat_[2][Y] -
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[2][Z] +
      t_m.m_mat_[1][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[2][Y] +
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Y] * t_m.m_mat_[1][Z] -
      t_m.m_mat_[2][X] * t_m.m_mat_[0][Z] * t_m.m_mat_[1][Y];

    auto det = t_m.m_mat_[0][X] * inv.m_mat_[0][X] 
      + t_m.m_mat_[0][Y] * inv.m_mat_[1][X] 
      + t_m.m_mat_[0][Z] * inv.m_mat_[2][X] 
      + t_m.m_mat_[0][T] * inv.m_mat_[3][X];
    det = 1.0f / det;
    inv *= det;
    return inv;
  }

  Mat4 Mat4::ortho(const float t_left, const float t_right
    , const float t_bottom, const float t_top, const float t_near, const float t_far)
  {
    Mat4 result(1);
    result.m_mat_[0][X] = 2.0f / (t_right - t_left);
    result.m_mat_[1][Y] = 2.0f / (t_top - t_bottom);
    result.m_mat_[2][Z] = 2.0f / (t_near - t_far);
    result.m_mat_[0][T] = (t_left + t_right) / (t_left - t_right);
    result.m_mat_[1][T] = (t_bottom + t_top) / (t_bottom - t_top);
    result.m_mat_[2][T] = (t_far + t_near) / (t_far - t_near);
    return result;
  }

  Mat4 Mat4::perspective(const float t_fov, const float t_aspect_ratio
    , const float t_near, const float t_far)
  {
    Mat4 result(1);
    const auto TO_RADIAN = 1.0f / 180.0f * 3.14f;
    result.m_mat_[1][Y] = 1.0f / tan(0.5f * t_fov * TO_RADIAN);
    result.m_mat_[0][X] = result.m_mat_[1][Y] / t_aspect_ratio;

    result.m_mat_[2][Z] = (t_near + t_far) / (t_near - t_far);
    result.m_mat_[3][Z] = -1;
    result.m_mat_[2][T] = 2.0f * t_near * t_far / (t_near - t_far);
    return result;
  }

  // ___ constructor __________________________________________________________
  Mat4::Mat4()
  {
    for (auto& row : m_mat_)
    {
      for (auto& col : row)
      {
        col = 0.0f;
      }
    }
  }
  
  Mat4::Mat4(const float t_diagonal)
  {
    for(auto i = 0; i < SIZE; ++i)
    {
      m_mat_[i][i] = t_diagonal;
    }
  }

  Mat4::Mat4(const float(& t_array_2_d)[SIZE][SIZE])
  {
    for (auto row = 0; row < SIZE; ++row)
    {
      for (auto col = 0; col < SIZE; ++col)
      {
        m_mat_[row][col] = t_array_2_d[row][col];
      }
    }
  }

  Mat4::Mat4(const Vec4<float>& t_row0, const Vec4<float>& t_row1
    , const Vec4<float>& t_row2, const Vec4<float>& t_row3)
  {
    m_mat_[0][X] = t_row0.x;
    m_mat_[0][Y] = t_row0.y;
    m_mat_[0][Z] = t_row0.z;
    m_mat_[0][T] = t_row0.t;

    m_mat_[1][X] = t_row1.x;
    m_mat_[1][Y] = t_row1.y;
    m_mat_[1][Z] = t_row1.z;
    m_mat_[1][T] = t_row1.t;

    m_mat_[2][X] = t_row2.x;
    m_mat_[2][Y] = t_row2.y;
    m_mat_[2][Z] = t_row2.z;
    m_mat_[2][T] = t_row2.t;

    m_mat_[3][X] = t_row3.x;
    m_mat_[3][Y] = t_row3.y;
    m_mat_[3][Z] = t_row3.z;
    m_mat_[3][T] = t_row3.t;
  }

  // ___ operators __________________________________________________________
  bool Mat4::operator==(const Mat4& t_other) const
  {
    for (auto row = 0; row < SIZE; ++row)
    {
      for (auto col = 0; col < SIZE; ++col)
      {
        if (abs(m_mat_[row][col] - t_other.m_mat_[row][col]) 
          > Game_engine::PRECISION)
        {
          return false;
        }
      }//col loop
    }
    return true;
  }

  Mat4& Mat4::operator*=(const Mat4& t_other)
  {
    return *this = *this * t_other;
  }

  Mat4& Mat4::operator*=(const float t_scalar)
  {
    for (auto& row : m_mat_)
    {
      for (auto& col : row)
      {
        col *= t_scalar;
      }
    }
    return *this;
  }

  Mat4 Mat4::operator*(const Mat4& t_other) const
  {
    Mat4 temp;
    for (auto this_row = 0; this_row < SIZE; ++this_row)
    {
      for (auto other_col = 0; other_col < SIZE; ++other_col)
      {
        for (auto this_col_other_row = 0; this_col_other_row < SIZE; ++this_col_other_row)
        {
          temp.m_mat_[this_row][other_col] += m_mat_[this_row][this_col_other_row]
            * t_other.m_mat_[this_col_other_row][other_col];
        }
      }
    }
    return temp;
  }

  Mat4 Mat4::operator*(const float t_scalar) const
  {
    Mat4 temp;
    for (auto row = 0; row < SIZE; ++row)
    {
      for (auto col = 0; col < SIZE; ++col)
      {
        temp.m_mat_[row][col] = m_mat_[row][col] * t_scalar;
      }
    }
    return temp;
  }

  Vec3<float> Mat4::operator*(const Vec3<float>& t_other) const
  {
    Vec3<float> temp;
    for (auto this_row = 0; this_row < SIZE; ++this_row)
    {
      temp.x += m_mat_[this_row][X] * t_other.x;
      temp.y += m_mat_[this_row][Y] * t_other.y;
      temp.z += m_mat_[this_row][Z] * t_other.z;
    }
    return temp;
  }

  // ___ friend _________________________________________________________________
  Vec4<float> operator*(const Mat4& t_left, const Vec4<float>& t_right)
  {
    Vec4<float> temp;
    for (auto row = 0; row < Mat4::SIZE; ++row)
    {
      temp.x += t_left.m_mat_[row][Mat4::X] * t_right.x;
      temp.y += t_left.m_mat_[row][Mat4::Y] * t_right.y;
      temp.z += t_left.m_mat_[row][Mat4::Z] * t_right.z;
      temp.t += t_left.m_mat_[row][Mat4::T] * t_right.t;
    }
    return temp;
  }

  // ___ public _________________________________________________________________
  std::string Mat4::to_string(const bool t_shows_float) const
  {
    std::string str;
    for (auto& row : m_mat_)
    {
      str += '\n';
      for (auto& col : row)
      {
        const auto points = 2;
        str += ' '+ Game_engine::double_points_string(col, t_shows_float? points: 0);
      }
    }
    str += '\n';

    return str;
  }

  Vec3<float> Mat4::get_multiplied(const Vec3<float>& t_other) const
  {
    return *this*t_other;
  }

  Vec4<float> Mat4::get_multiplied(const Vec4<float>& t_other) const
  {
    return *this*t_other;
  }

  void Mat4::multiply(const Mat4& t_other)
  {
    *this *= t_other;
  }

  void Mat4::invert()
  {
    *this = inverse(*this);
  }

  void Mat4::set(const Vec2<int>& t_pos, const float t_value)
  {
    m_mat_[t_pos.y][t_pos.x] = t_value;
  }

  const float* Mat4::to_array() const
  {
    const auto array = new float[SIZE*SIZE];
    for(auto row = 0; row < SIZE; ++row)
    {
      for (auto col = 0; col < SIZE; ++col)
      {
        array[row * SIZE + col] = m_mat_[row][col];
      }
    }
    return array;
  }
}//jdb