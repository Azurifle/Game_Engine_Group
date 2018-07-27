#ifndef JDB_ENGINE_HPP
#define JDB_ENGINE_HPP
#pragma once
#include "Delta_time.hpp"
#include "my_math/Vec3.hpp"
#include "thirdparty/json.hpp"

namespace jdb
{
  class System_base;

  class Engine final
  {
  public:
    // ___ Utility _____________________________________________________________________________

    static const float PRECISION;

    enum Default { DOUBLE_POINTS = 2 };
    static std::string double_points_string(double t_double
      , int t_points = DOUBLE_POINTS);

    static void load_txt(const std::string& t_path, std::string& t_string_out);
    static void load_bmp(const std::string& t_path, std::vector<std::vector<Vec3<int>>>& t_image);
    static nlohmann::json load_json(const std::string& t_path);

	static int random(int t_min, int t_max);

    // ___ Engine _____________________________________________________________________________

    enum State { INVALID, CONSTRUCTING, INITIALIZING, RUNNING, SHUTTING_DOWN, DESTROYING };
    static State state();

    Engine();
    ~Engine();

    void run(std::unique_ptr<System_base> t_app);
  private:
    static State m_state_;

    static void disable_mouse_editing();
    static void show_header();
    GLFWwindow* init();

    static void key_callback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action
      , int t_mods);

    std::unique_ptr<System_base> m_app_{};
    Delta_time m_delta_time_;

    Engine(const Engine& t_to_copy) = default;
    Engine(Engine&& t_to_move) noexcept = default;
    Engine& operator=(const Engine& t_to_copy) = default;
    Engine& operator=(Engine&& t_to_move) noexcept = default;
  };
}//jdb

#endif //JDB_ENGINE_HPP