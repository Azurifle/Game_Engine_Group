#ifndef G6037599_CONSOLE_HPP
#define G6037599_CONSOLE_HPP
#pragma once
#include "Hp_bar.hpp"
#include "Status_panel.hpp"

namespace jdb
{
  class Timer;

  class Console final
  {
  public:
    const enum Enum { SPACE_BETWEEN_TILE = 3, POP_UP_PANEL_WIDTH = 65 };

    static COORD get_cursor();
    static void set_cursor(const COORD& t_pos);
    static void print_centered_text(int t_width, const char* t_message, char t_delim);
    
    Console();
    ~Console() = default;
    
    void show() const;
    void print_player_status() const;
    void print_map() const;
    void marked(const COORD& t_pos, const char* t_symbol, bool t_is_attacker = false) const;
    void move_player_cursor(const COORD& t_pos);
    int update_minute() const;
    int update_hour() const;
    void hide_cursor_status() const;
    void show_cursor_status(const std::string& t_name
      , int t_hp, int t_max_hp, int t_atk, int t_max_atk) const;
    void hide_monster_status() const;
    void show_monster_status(const std::string& t_name
      , int t_hp, int t_max_hp, int t_atk, int t_max_atk) const;
    static void thanks_user();
    void show_game_reset() const;
    void show_count_down(int t_count_down) const;
    void show_next_stage() const;
    void set_player_full_hp() const;
    void set_player_hp(int t_hp) const;
    void set_monster_hp(int t_hp, int t_max_hp) const;
    void set_cursor_hp(int t_hp, int t_max_hp) const;
  private:
    const char* GAME_RESET_MESSAGE = "Game reset in ?.";
    COORD m_map_start_ = { 0, 0 }, m_player_cursor_ = { 0, 0 }
      , m_pop_up_panel_cursor_ = {0, 0}, m_game_reset_count_down_cursor_ = { 0, 0 };
    std::unique_ptr<Status_panel> m_monster_status_ = nullptr;
    std::unique_ptr<Status_panel> m_cursor_status_ = nullptr;
    std::unique_ptr<Hp_bar> m_player_hp_ = nullptr;
    std::unique_ptr<Timer> m_timer_ = nullptr;

    Console(const Console& t_to_copy) = default;
    Console(Console&& t_to_move) noexcept = default;
    Console& operator=(const Console& t_to_copy) = default;
    Console& operator=(Console&& t_to_move) noexcept = default;

    COORD find_map_cursor_pos(const COORD& t_pos) const;
  };

}//jdb

#endif //G6037599_CONSOLE_HPP