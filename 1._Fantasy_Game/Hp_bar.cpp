#include "stdafx.h"
#include "Hp_bar.hpp"
#include "Console.hpp"

namespace G6037599
{
  //___ (de)constructors _____________________________________________
  Hp_bar::Hp_bar(const COORD& t_pos) : m_start_(t_pos) {}

  //___ public _____________________________________________
  COORD Hp_bar::get_pos() const
  {
    return m_start_;
  }

  void Hp_bar::set(const int t_hp, const int t_max_hp) const
  {
    const auto HP_DISPLAY_LIMIT = 1000;
    REQUIRE(0 <= t_hp && t_hp <= t_max_hp && t_max_hp < HP_DISPLAY_LIMIT);

    Console::set_cursor(m_start_);
    std::cout << '[';
    const auto MAX_BAR = 6;
    auto bar_left = MAX_BAR * t_hp / t_max_hp;

    bar_left = print_half_bar(bar_left);

    const auto DIGIT = std::to_string(t_max_hp).length();
    std::cout << "HP:" << std::setw(DIGIT) << std::setfill('0') << t_hp
      << '/' << std::setw(DIGIT) << std::setfill('0') << t_max_hp;

    print_half_bar(bar_left);

    std::cout << ']';
  }

  //___ private static _____________________________________________
  int Hp_bar::print_half_bar(int t_bar_left)
  {
    const enum Bar {NO_BAR, BAR_1, HALF_BAR = 3};
    for (int i = BAR_1; i <= HALF_BAR; ++i)
    {
      if (t_bar_left > NO_BAR)
      {
        std::cout << '#';
        --t_bar_left;
      }
      else
      {
        std::cout << ' ';
      }
    }

    return t_bar_left;
  }

}//G6037599
