#ifndef SHADOW_MAZE_GAME_HPP
#define SHADOW_MAZE_GAME_HPP
#pragma once

namespace shadow_maze
{
  class Game final
  {
  public:
    static void init();

    ~Game();
  private:
    static const std::string GAME_FOLDER;

    Game();
    Game(const Game& t_to_copy) = default;
    Game(Game&& t_to_move) noexcept = default;
    Game& operator=(const Game& t_to_copy) = default;
    Game& operator=(Game&& t_to_move) noexcept = default;
  };
}//shadow_maze

#endif//SHADOW_MAZE_GAME_HPP
