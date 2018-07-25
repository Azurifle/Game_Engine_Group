#ifndef JDB_GAME_OBJECT_HPP
#define JDB_GAME_OBJECT_HPP
#pragma once

namespace jdb
{
  class Game_object
  {
  public:
    Game_object() = default;
    virtual ~Game_object() = default;
    Game_object(const Game_object& t_to_copy) = default;
    Game_object(Game_object&& t_to_move) noexcept = default;
    Game_object& operator=(const Game_object& t_to_copy) = default;
    Game_object& operator=(Game_object&& t_to_move) noexcept = default;

    virtual void start() = 0;
    virtual void update() = 0;
  };
}//jdb

#endif//JDB_GAME_OBJECT_HPP
