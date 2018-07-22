#ifndef JDB_GAME_ENGINE_HPP
#define JDB_GAME_ENGINE_HPP
#pragma once
#include "My_Math/Vec3.hpp"

namespace jdb
{
  class Grid;

  class Engine final
  {
  public:
    enum Enum
    {
      KEY_NO_PRESS
      , KEY_ARROW = 224
      , KEY_ARROW_UP = 72, KEY_ARROW_DOWN = 80
      , KEY_ARROW_LEFT = 75, KEY_ARROW_RIGHT = 77
      , KEY_SPACE = 32
      , KEY_ESC = 27
      , FPS_50 = 20
      , CMD_LAST_COLS = 140, CMD_LAST_ROWS = 40
      , DEFAULT_DOUBLE_POINTS = 2
    };
    static const float SECOND, PRECISION;
    static const Vec3<float> WHITE;

    static void start();
    static bool is_running();

    static int get_key();
    static int wait_key();
    static int wait_key(int t_miliseconds);
    static COORD get_cursor();
    static void set_cursor(const COORD& t_coord);

    static short limit_interval(short t_number, short t_min, short t_max);

    static void load_txt(const std::string& t_path, std::string& t_string_out);
    static void load_bmp(const std::string& t_path
      , std::vector<std::vector<std::vector<int>>>& t_image);

    static int random(int t_min, int t_max);

    static void reset_delta_time();
    static float get_delta_time();

    static std::string double_points_string(double t_double
      , int t_points = DEFAULT_DOUBLE_POINTS);

    static void paint_pos(const Vec3<float>& t_pos, const Vec3<float>& t_rgb);
    
    ~Engine() = default;
  private:
    static clock_t m_delta_milisec_;
    static bool m_is_running_;

    static void disable_mouse_editing();
    static void show_header();
    static void back_to_main_menu();

    Engine() = default;
    Engine(const Engine& t_to_copy) = default;
    Engine(Engine&& t_to_move) noexcept = default;
    Engine& operator=(const Engine& t_to_copy) = default;
    Engine& operator=(Engine&& t_to_move) noexcept = default;
  };
}//jdb

#endif //JDB_GAME_ENGINE_HPP