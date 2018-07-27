#ifndef SHADOW_MAZE_PLAYER_HPP
#define SHADOW_MAZE_PLAYER_HPP
#pragma once
#include <jdb_engine/my_math/Vec3.hpp>
#include "Map.hpp"

namespace shadow_maze
{
  class Player final
  {
  public:
    Player();
    ~Player() = default;
    Player(const Player& t_to_copy) = default;
    Player(Player&& t_to_move) noexcept = default;
    Player& operator=(const Player& t_to_copy) = default;
    Player& operator=(Player&& t_to_move) noexcept = default;

    void spawn_on(const std::shared_ptr<Map>& t_map);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
  private:
    enum State { NOT_SPAWN = -1 };

    jdb::Vec3<float> m_pos_;
    std::shared_ptr<Map> m_map_;
  };
}//shadow_maze

#endif//SHADOW_MAZE_PLAYER_HPP