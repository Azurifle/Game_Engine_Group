#ifndef G6037599_MAT4_HPP
#define G6037599_MAT4_HPP
#pragma once
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Vec2.hpp"

namespace jdb
{
  class Mat4 final
  {
  public:
    enum Enum { X, Y, Z, T, SIZE};

    static Mat4 identity();
    static Mat4 translation(const Vec3<float>& t_translate);
    static Mat4 rotation(const Vec3<float>& t_radian_angles);
    static Mat4 scaling(const Vec3<float>& t_scale);
    static Mat4 transpose(const Mat4& t_matrix);
    static Mat4 inverse(const Mat4& t_m);
    static Mat4 ortho(float t_left, float t_right, float t_bottom, float t_top, float t_near, float t_far);
    static Mat4 perspective(float t_fov, float t_aspect_ratio, float t_near, float t_far);

    Mat4();
    explicit Mat4(float t_diagonal);
    explicit Mat4(const float (&t_array_2_d)[SIZE][SIZE]);
    Mat4(const Vec4<float>& t_row0, const Vec4<float>& t_row1
      , const Vec4<float>& t_row2, const Vec4<float>& t_row3);
    ~Mat4() = default;
    Mat4(const Mat4& t_to_copy) = default;
    Mat4(Mat4&& t_to_move) noexcept = default;
    Mat4& operator = (const Mat4& t_to_copy) = default;
    Mat4& operator = (Mat4&& t_to_move) noexcept = default;

    bool operator == (const Mat4& t_other) const;
    Mat4& operator *= (const Mat4& t_other);
    Mat4& operator *= (float t_scalar);
    Mat4 operator * (const Mat4& t_other) const;
    Mat4 operator * (float t_scalar) const;
    Vec3<float> operator * (const Vec3<float>& t_other) const;

    friend Vec4<float> operator * (const Mat4& t_left, const Vec4<float>& t_right);

    std::string to_string(bool t_shows_float = false) const;
    Vec3<float> get_multiplied(const Vec3<float>& t_other) const;
    Vec4<float> get_multiplied(const Vec4<float>& t_other) const;
    void multiply(const Mat4& t_other);
    void invert();
    void set(const Vec2<int>& t_pos, float t_value);
    const float* to_array() const;
  private:
    float m_mat_[SIZE][SIZE]{};
  };
}//jdb

#endif //G6037599_MAT4_HPP
