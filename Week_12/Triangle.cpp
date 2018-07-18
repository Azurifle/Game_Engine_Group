#include <stdafx.h>
#include "Triangle.hpp"
#include "Demo_Center/Demo_Center.hpp"

namespace G6037599
{
  // ___ constructor __________________________________________________________
  Triangle::Triangle(const Vec3<float>& t_pos, const Vec3<float>& t_scale
    , const float t_z_degree, const float t_rotate_speed) : pos(t_pos), scale(t_scale)
    , z_degree(t_z_degree), rotate_speed(t_rotate_speed), m_x_degree_(0) {}

  // ___ public ______________________________________________________________
  void Triangle::update(const float t_delta_seconds)
  {
    m_x_degree_ += rotate_speed * t_delta_seconds;

    glPushMatrix();
      glRotatef(z_degree, 0, 0, 1);
      glTranslatef(pos.x, pos.y, pos.z);
      glScalef(scale.x, scale.y, scale.z);

      glPushMatrix();
        glRotatef(m_x_degree_, 1, 0, 0);//world coord in->out
        draw();
      glPopMatrix();
    glPopMatrix();
  }

  // ___ private ________________________________________________________
  void Triangle::draw() const
  {
    auto color = Demo_center::WHITE;
    static const auto PI = 3.14f, DEGREE_TO_RADIAN = 1/ 360.0f*(2 * PI);
    color.z = -cos(m_x_degree_*DEGREE_TO_RADIAN);

    glBegin(GL_TRIANGLES);
      Demo_center::paint_pos(Vec3<float>(-1, 0, 0), color);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
  }

}//G6037599