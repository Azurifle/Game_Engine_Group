#ifndef JDB_APP_HPP
#define JDB_APP_HPP
#pragma once
#include "0._Game_Engine/Delta_time.hpp"

namespace jdb
{
  class Shadow_maze final
  {
  public:
    static void run();

    ~Shadow_maze();
  private:
    static const std::string GAME_FOLDER;

    Delta_time m_delta_time_;

    Shadow_maze();
    Shadow_maze(const Shadow_maze& t_to_copy) = default;
    Shadow_maze(Shadow_maze&& t_to_move) noexcept = default;
    Shadow_maze& operator=(const Shadow_maze& t_to_copy) = default;
    Shadow_maze& operator=(Shadow_maze&& t_to_move) noexcept = default;

    void update();
  };
}//jdb

#endif//JDB_APP_HPP
