#ifndef SHADOW_MAZE_GAME_HPP
#define SHADOW_MAZE_GAME_HPP
#pragma once
#include <jdb_engine/System_base.hpp>
#include <jdb_engine/visual/Mesh.hpp>
#include <jdb_engine/visual/Texture_manager.hpp>
#include <jdb_engine/Audio_manager.hpp>

namespace shadow_maze
{
  class Game final : public jdb::System_base
  {
  public:
    Game() = default;
    ~Game();
    Game(Game&& t_to_move) noexcept = default;
    Game& operator=(Game&& t_to_move) noexcept = default;

    // ___ setup glfw window _____________________________________________________________________

    std::string title() override;
    void window_size(jdb::Vec2<int>& t_out) override;
    void key_callback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action
      , int t_mods) override;

    // ___ game play _____________________________________________________________________________

    void start() override;
    void update() override;
  private:
    enum Window_size { WIDTH = 800, HEIGHT = 600 };
    static const std::string TEXTURE_FOLDER;
    void load_texture(const std::string& t_png_name, GLenum t_color_format = GL_RGB);
    std::vector<jdb::Texture> m_textures_{};

    std::shared_ptr<jdb::Mesh> m_test_mesh_{}, m_tile_mesh_{};
	std::vector<std::shared_ptr<jdb::Mesh>> m_player_mesh_;
    void setup_meshes();
    void update_game_objects() const;
    jdb::Audio m_bgm_{};

    Game(const Game& t_to_copy) = default;
    Game& operator=(const Game& t_to_copy) = default;
  };
}//shadow_maze

#endif//SHADOW_MAZE_GAME_HPP
