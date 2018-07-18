#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP
#pragma once
#include "My_Math/Vec3.hpp"

namespace jdb
{
  class Grid;

  class System_base
  {
  public:
	  virtual void init(bool) { std::cout << "SystemBase init\n"; Exit(); }
	  virtual void run() { std::cout << "SystemBase update\n"; Exit(); }
	  virtual void getInput(int) { std::cout << "SystemBase getInput\n"; Exit(); }
  private:
	  void Exit();
  };

  enum Engine_state
  {
	  INVALID,
	  CONSTRUCTING,
	  INITIALIZING,
	  RUNNING,
	  SHUTTING_DOWN,
	  DESTROYING
  };

  class Game_engine final
  {
  public:

	Game_engine() = default;
    enum Keyboard_input
    {
      KEY_NO_PRESS
      , KEY_ARROW = 224
      , KEY_ARROW_UP = 72, KEY_ARROW_DOWN = 80
      , KEY_ARROW_LEFT = 75, KEY_ARROW_RIGHT = 77
      , KEY_SPACE = 32
      , KEY_ESC = 27
    };
	enum Enum
	{
		FPS_50 = 20
		, CMD_LAST_COLS = 140, CMD_LAST_ROWS = 40
		, DEFAULT_DOUBLE_POINTS = 2
	};
    static const float SECOND, PRECISION;
    static const Vec3<float> WHITE;

	int start();
    static bool is_running();

    static int get_key();
    static int wait_key();
    static int wait_key(int t_miliseconds);
    static COORD get_cursor();
    //static char get_cursor_char();
    static void set_cursor(const COORD& t_coord);

    static short limit_interval(short t_number, short t_min, short t_max);

    static void load_txt(const std::string& t_path
      , std::vector<std::string>& t_tokens_out);
    static void load_bmp(const std::string& t_path
      , std::vector<std::vector<std::vector<int>>>& t_image);

    static int random(int t_min, int t_max);

    static void reset_delta_time();
    static float get_delta_time();

    static std::string double_points_string(double t_double
      , int t_points = DEFAULT_DOUBLE_POINTS);

    static void paint_pos(const Vec3<float>& t_pos, const Vec3<float>& t_rgb);
    
	//
	
	static void shutdown_engine() { 
		m_Enging_state = Engine_state::SHUTTING_DOWN;
		back_to_main_menu();
	}


    ~Game_engine() = default;
  private:
	static Engine_state m_Enging_state;
	//static System_base game_system;

	int intialize(std::shared_ptr<System_base> system);
	int update(std::shared_ptr<System_base> system);
	std::shared_ptr<System_base> console_menu();



    static clock_t m_delta_milisec_;
    static bool m_is_running_;

    static void disable_mouse_editing();
    static void show_header();
    static void back_to_main_menu();

    Game_engine(const Game_engine& t_to_copy) = default;
    Game_engine(Game_engine&& t_to_move) noexcept = default;
    Game_engine& operator=(const Game_engine& t_to_copy) = default;
    Game_engine& operator=(Game_engine&& t_to_move) noexcept = default;
  };
}//jdb

#endif //GAME_ENGINE_HPP