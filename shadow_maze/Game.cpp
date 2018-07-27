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
    switch (t_action)
    {
    case GLFW_PRESS: case GLFW_REPEAT: 
      switch_key(t_key, t_window, jdb::Vec2<int>(t_scancode, t_mods)); default:;
    }
  }

  void Game::start()
  {
    m_map_.init("shadow_maze/maze/01.bmp");

    calulate_map_coord();

    jdb::Audio_manager::play(m_bgm_);
  }

  void Game::update(const float t_delta_secs)
  {
    static const jdb::Vec3<float> BG_COLOR(
      m_config_["bg_color.r"], m_config_["bg_color.g"], m_config_["bg_color.b"]);
    jdb::Renderer::render_bg(BG_COLOR);

    jdb::Renderer::set_draw_frame(m_map_coord_, m_window_size_*2);
    m_map_.render_mini();

    jdb::Renderer::set_draw_frame(jdb::Vec2<int>(), m_window_size_/4);
    m_map_.render_mini();
  }

  // ___ private _________________________________________________________________________________

  void Game::switch_key(const int t_key, GLFWwindow* t_window
    , const jdb::Vec2<int>& t_scancode_n_mods)
  {
    switch(t_key)
    {
    case GLFW_KEY_F3: show_window_info(t_window, t_scancode_n_mods); break;
    case GLFW_KEY_W: m_map_.move_player(Map::Face::UP); calulate_map_coord(); break;
    case GLFW_KEY_S: m_map_.move_player(Map::Face::DOWN); calulate_map_coord(); break;
    case GLFW_KEY_A: m_map_.move_player(Map::Face::LEFT); calulate_map_coord(); break;
    case GLFW_KEY_D: m_map_.move_player(Map::Face::RIGHT); calulate_map_coord(); default:;
    }
  }

  void Game::show_window_info(GLFWwindow* t_window, const jdb::Vec2<int>& t_scancode_n_mods)
  {
    puts("F3 pressed: Show GLFW window information");
    std::cout << "window:" << t_window << ", scancode: " << t_scancode_n_mods.x
      << ", mods: " << t_scancode_n_mods.y << std::endl;
    std::cout << "WIDTH:" << m_config_["window_width"]
      << ", HEIGHT: " << m_config_["window_height"] << std::endl;
  }

  void Game::calulate_map_coord()
  {
    m_map_coord_.x = m_window_size_.x / 2 - m_map_.player_pos_ratio().x*m_window_size_.x * 2;
    m_map_coord_.y = m_window_size_.y / 2 - m_map_.player_pos_ratio().y * m_window_size_.y * 2;
  }

}//shadow_maze