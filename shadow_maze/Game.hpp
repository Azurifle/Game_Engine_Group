#ifndef SHADOW_MAZE_GAME_HPP
#define SHADOW_MAZE_GAME_HPP
#pragma once
#include <jdb_engine/System_base.hpp>
#include <jdb_engine/thirdparty/json.hpp>
#include <jdb_engine/Audio_manager.hpp>
#include "Map.hpp"

namespace shadow_maze
{
  class Game final : public jdb::System_base
  {
  public:
    Game();
    ~Game();
    Game(Game&& t_to_move) noexcept = default;
    Game& operator=(Game&& t_to_move) noexcept = default;

    std::string title() override;
    jdb::Vec2<int> window_size() override;
    void key_callback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action
      , int t_mods) override;
    void calulate_map_coord();
    void start() override;
    void update(float t_delta_secs) override;
  private:
    nlohmann::json m_config_{};
    Map m_map_;
    jdb::Vec2<int> m_window_size_{}, m_map_coord_{};
    jdb::Audio m_bgm_{};

    Game(const Game& t_to_copy) = default;
    Game& operator=(const Game& t_to_copy) = default;

    void switch_key(int t_key, GLFWwindow* t_window, const jdb::Vec2<int>& t_scancode_n_mods);
    void show_window_info(GLFWwindow* t_window, const jdb::Vec2<int>& t_scancode_n_mods);
  };
}//shadow_maze

#endif//SHADOW_MAZE_GAME_HPP