#ifndef G6037599_HP_BAR_HPP
#define G6037599_HP_BAR_HPP
#pragma once

namespace jdb
{
  class Hp_bar final
  {
  public:
    explicit Hp_bar(const COORD& t_pos);
    ~Hp_bar() = default;
    Hp_bar(const Hp_bar& t_to_copy) = default;//no (smart)pointer
    Hp_bar(Hp_bar&& t_to_move) noexcept = default;
    Hp_bar& operator=(const Hp_bar& t_to_copy) = default;
    Hp_bar& operator=(Hp_bar&& t_to_move) noexcept = default;

    COORD get_pos() const;
    void set(int t_hp, int t_max_hp) const;
  private:
    COORD m_start_{};

    static int print_half_bar(int t_bar_left);
  };
}//jdb

#endif //G6037599_HP_BAR_HPP

