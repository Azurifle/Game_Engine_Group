#include <stdafx.h>
#include "Game.hpp"
#include <jdb_engine/Engine.hpp>
#include <jdb_engine/Gui_system.hpp>
#include <jdb_engine/visual/Renderer.hpp>

namespace shadow_maze
{
  // ___ public _____________________________________________________________________

  //How to use json: https://github.com/nlohmann/json/tree/master

  Game::Game(): m_config_(jdb::Engine::load_json("shadow_maze/Config.json"))
    , m_window_size_(jdb::Vec2<int>(m_config_["window_width"], m_config_["window_height"]))
    , m_bgm_(jdb::Audio_manager::load_or_get_audio("shadow_maze/music/Game.wav")) {}

  Game::~Game()
  {
    jdb::Audio_manager::stop(m_bgm_);
  }

  std::string Game::title() { return m_config_["title"]; }

  jdb::Vec2<int> Game::window_size() { return  m_window_size_; }

  void Game::key_callback(GLFWwindow* t_window, const int t_key, const int t_scancode
    , const int t_action, const int t_mods)
  {
    if (t_key == GLFW_KEY_F3 && t_action == GLFW_PRESS)
    {
      puts("F3 pressed: Show GLFW window information");
      std::cout << "window:" << t_window << ", scancode: " << t_scancode 
        << ", mods: " << t_mods << std::endl;
	    std::cout << "WIDTH:" << m_config_["window_width"]
        << ", HEIGHT: " << m_config_["window_height"] << std::endl;
    }
  }

  void Game::start()
  {
    m_map_.pre_render();
    m_map_.load("shadow_maze/maze/01.bmp");
    jdb::Audio_manager::play(m_bgm_);
  }

  void Game::update(const float t_delta_secs)
  {
    static const jdb::Vec3<float> BG_COLOR(
      m_config_["bg_color.r"], m_config_["bg_color.g"], m_config_["bg_color.b"]);
    jdb::Renderer::render_bg(BG_COLOR);

    m_map_.render();
  }

}//shadow_maze