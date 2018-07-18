#include <stdafx.h>
#include "Demo_Center.hpp"

namespace G6037599
{
  // ___ static ___________________________________________________________
  const float Demo_center::PRECISION = 0.01f;
  const Vec3<float> Demo_center::WHITE = Vec3<float>(1);

  void Demo_center::disable_mouse_editing()
  {
    DWORD prev_mode;
    PROMISE(GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &prev_mode));
    PROMISE(SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), prev_mode & ~ENABLE_QUICK_EDIT_MODE));
  }

  int Demo_center::get_key()
  {
    switch (_kbhit())
    {
    case KEY_NO_PRESS: return KEY_NO_PRESS;
    default: return wait_key();
    }
  }

  int Demo_center::wait_key()
  {
    const auto FIRST_KEY = _getch();
    clear_2_nd_key_if_has();
    return FIRST_KEY;
  }

  void Demo_center::press_to_continue()
  {
    puts("");
    const std::string HINT("Press <Any key> to continue: ");
    std::cout << HINT.c_str();
    wait_key();
    std::cout << '\r' << std::setw(HINT.size()) << std::setfill(' ')
      << ' ' << '\r';
  }

  void Demo_center::print_centered_header(const std::string& t_header, const char t_delim)
  {
    const unsigned MAX_HEADER_COLS = 80
      , SPACE_BEFORE = (MAX_HEADER_COLS - t_header.length()) / 2
      , SPACE_AFTER = SPACE_BEFORE + SPACE_BEFORE % 2;
    std::cout << std::setw(SPACE_BEFORE-1) << std::setfill(t_delim) << t_delim;
    std::cout << ' ' << t_header << ' '
      << std::setw(SPACE_AFTER - 1) << std::setfill(t_delim) << t_delim << std::endl;
  }

  std::string Demo_center::double_points_string(const double t_double, const int t_points)
  {
    std::stringstream double_w_points;
    double_w_points << std::fixed << std::setprecision(t_points) << t_double;
    return double_w_points.str();
  }

  void Demo_center::paint_pos(const Vec3<float>& t_pos, const Vec3<float>& t_rgb)
  {
    glColor3f(t_rgb.x, t_rgb.y, t_rgb.z);
    glVertex3f(t_pos.x, t_pos.y, t_pos.z);
  }

  // ___ private static ________________________________________________
  void Demo_center::clear_2_nd_key_if_has()
  {
    switch (_kbhit())
    {
    case KEY_NO_PRESS: break;
    default: _getch();
    }
  }

}//G6037599