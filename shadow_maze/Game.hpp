#ifndef SHADOW_MAZE_GAME_HPP
#define SHADOW_MAZE_GAME_HPP
#pragma once
#include <jdb_engine/System_base.hpp>
#include <jdb_engine/visual/Mesh.hpp>
#include <jdb_engine/Audio_manager.hpp>
#include <jdb_engine/thirdparty/json.hpp>
#include "jdb_engine/Game_object.hpp"

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
    void start() override;
    void update() override;
  private:
    nlohmann::json m_config_{};
    jdb::Vec2<int> m_window_size_{};

    enum Object { GRASS_TILE, TEST, SIZE };
    std::vector<jdb::Game_object> m_game_objects_{};
    jdb::Audio m_bgm_{};

    jdb::Texture load_texture(const std::string& t_png_name, GLenum t_color_format = GL_RGB);
    void setup_meshes();

    Game(const Game& t_to_copy) = default;
    Game& operator=(const Game& t_to_copy) = default;
  };
}//shadow_maze

#endif//SHADOW_MAZE_GAME_HPP