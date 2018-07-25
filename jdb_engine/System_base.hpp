#ifndef JDB_SYSTEM_BASE_HPP
#define JDB_SYSTEM_BASE_HPP
#pragma once
#include "my_math/Vec2.hpp"

namespace jdb
{
  class System_base
  {
  public:
    System_base() = default;
    virtual ~System_base() = default;
    System_base(System_base&& t_to_move) noexcept = default;
    System_base& operator=(System_base&& t_to_move) noexcept = default;

    // ___ setup glfw window _____________________________________________________________________

    virtual std::string title() = 0;
    virtual Vec2<int> window_size() = 0;
    virtual void key_callback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action
      , int t_mods) = 0;

    // ___ game play _____________________________________________________________________________

    virtual void start() = 0;
    virtual void update() = 0;
  protected:
    System_base(const System_base& t_to_copy) = default;
    System_base& operator=(const System_base& t_to_copy) = default;
  };
}//jdb

#endif //JDB_SYSTEM_BASE_HPP