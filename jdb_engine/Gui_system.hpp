#ifndef JDB_GUI_SYSTEM_HPP
#define JDB_GUI_SYSTEM_HPP
#pragma once
#include "my_math/Vec2.hpp"

namespace jdb
{
  class Engine;

  class Gui_system final
  {
  public:
    static GLFWwindow* create_window(const std::string& t_title, const Vec2<int>& t_size);
    static void set_key_callback(GLFWwindow* t_window, Engine* t_engine
      , void(*t_key_callback)(struct GLFWwindow*, int, int, int, int));
    static bool has_window_close(GLFWwindow* t_window);
    static void swap_buffer_n_clear_inputs(GLFWwindow* t_window);
    static void destroy_windows();
  private:
    static void init_glew();
    static void show_glew_versions();
    static void test_depth_n_alpha_bend();

    Gui_system() = default;
    ~Gui_system() = default;
    Gui_system(const Gui_system& t_to_copy) = default;
    Gui_system(Gui_system&& t_to_move) noexcept = default;
    Gui_system& operator = (const Gui_system& t_to_copy) = default;
    Gui_system& operator = (Gui_system&& t_to_move) noexcept = default;
  };
}//jdb

#endif //JDB_GUI_SYSTEM_HPP
