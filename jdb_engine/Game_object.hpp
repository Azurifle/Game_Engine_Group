#ifndef JDB_GAME_OBJECT_HPP
#define JDB_GAME_OBJECT_HPP
#pragma once
#include "my_math/Vec3.hpp"
#include "visual/Texture_manager.hpp"

namespace jdb
{
  class Mesh;

  class Game_object final
  {
  public:
    std::shared_ptr<Mesh> mesh{};
    Texture texture{};
    Vec3<float> pos{}, radian_angles{}, scale{};

    Game_object();
    ~Game_object() = default;
    Game_object(const Game_object& t_to_copy) = default;
    Game_object(Game_object&& t_to_move) noexcept = default;
    Game_object& operator=(const Game_object& t_to_copy) = default;
    Game_object& operator=(Game_object&& t_to_move) noexcept = default;

    void render() const;
  };
}//jdb

#endif//JDB_GAME_OBJECT_HPP
