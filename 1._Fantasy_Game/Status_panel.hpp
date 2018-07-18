#ifndef G6037599_STATUS_PANEL_HPP
#define G6037599_STATUS_PANEL_HPP
#pragma once

namespace G6037599
{
  class Hp_bar;

  class Status_panel final
  {
  public:
    const enum Enum 
    { 
      MAX_NAME_LENGTH = 14, HIEGHT = 4, WIDTH = 20/* [###HP:999/999###] */
    };

    explicit Status_panel(COORD t_pos);
    ~Status_panel() = default;
    Status_panel(const Status_panel& t_to_copy);
    Status_panel(Status_panel&& t_to_move) noexcept = default;
    Status_panel& operator=(const Status_panel& t_to_copy);
    Status_panel& operator=(Status_panel&& t_to_move) noexcept = default;

    COORD get_pos() const;
    void show(const std::string& t_name, int t_hp, int t_max_hp, int t_atk, int t_max_atk);
    void hide();
    void set(int t_hp, int t_max_hp) const;
  private:
    COORD m_start_{};
    std::unique_ptr<Hp_bar> m_hp_{};
    bool m_is_hide_;
  };
}//G6037599

#endif //G6037599_STATUS_PANEL_HPP

