#include <stdafx.h>
#include "Game.hpp"
#include <jdb_engine/Engine.hpp>
#include <jdb_engine/Gui_system.hpp>
#include <jdb_engine/visual/Renderer.hpp>

namespace shadow_maze
{
  // ___ public _____________________________________________________________________

  //How to use json: https://github.com/nlohmann/json/tree/master

  Game::Game() : m_config_(jdb::Engine::load_json("shadow_maze/Config.json"))
    , m_mazes_(m_config_["mazes"].get<std::vector<std::string>>())
    , m_window_size_(jdb::Vec2<int>(m_config_["window_width"], m_config_["window_height"]))
    , m_bgm_(jdb::Audio_manager::load_or_get_audio("shadow_maze/music/Game.wav"))
    , m_win_bgm_(jdb::Audio_manager::load_or_get_audio("shadow_maze/music/Menu.wav"))
    , m_level_(0) {}

  Game::~Game()
  {
    jdb::Audio_manager::stop(m_bgm_); 
    jdb::Audio_manager::stop(m_win_bgm_);
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
    m_map_.init(m_mazes_[m_level_]);
    ++m_level_;
    calulate_map_coord();

    jdb::Audio_manager::play(m_bgm_);
  }
  void Game::update(const float t_delta_secs)
  {
    static const jdb::Vec3<float> BG_COLOR(
      m_config_["bg_color.r"], m_config_["bg_color.g"], m_config_["bg_color.b"]);
    jdb::Renderer::render_bg(BG_COLOR);

    jdb::Renderer::set_draw_frame(m_map_coord_, m_window_size_ * 2);
    m_map_.update(t_delta_secs);

    jdb::Renderer::set_draw_frame(jdb::Vec2<int>(), m_window_size_ / 4);
    m_map_.render_mini();
  }

  // ___ private _________________________________________________________________________________

  void Game::switch_key(const int t_key, GLFWwindow* t_window
    , const jdb::Vec2<int>& t_scancode_n_mods)
  {
    switch(t_key)
    {
    case GLFW_KEY_F3: show_window_info(t_window, t_scancode_n_mods); break;
    case GLFW_KEY_W: switch_move(m_map_.move_player(Map::Face::UP)); break;
    case GLFW_KEY_S: switch_move(m_map_.move_player(Map::Face::DOWN)); break;
    case GLFW_KEY_A: switch_move(m_map_.move_player(Map::Face::LEFT)); break;
    case GLFW_KEY_D: switch_move(m_map_.move_player(Map::Face::RIGHT)); default:;
    }
  }

  void Game::show_window_info(GLFWwindow* t_window
    , const jdb::Vec2<int>& t_scancode_n_mods)
  {
    puts("F3 pressed: Show GLFW window information");
    std::cout << "window:" << t_window << ", scancode: " << t_scancode_n_mods.x
      << ", mods: " << t_scancode_n_mods.y << std::endl;
    std::cout << "WIDTH:" << m_config_["window_width"]
      << ", HEIGHT: " << m_config_["window_height"] << std::endl;
  }

  void Game::calulate_map_coord()
  {
    m_map_coord_.x = static_cast<int>(round(m_window_size_.x / 2
      - m_map_.player_pos_ratio().x*m_window_size_.x * 2));
    m_map_coord_.y = static_cast<int>(round(m_window_size_.y / 2
      - m_map_.player_pos_ratio().y * m_window_size_.y * 2));
  }

  void Game::switch_move(const Map::Move_state t_move_state)
  {
    switch (t_move_state)
    {
    case Map::YES: calulate_map_coord(); break;
    case Map::WARP: if(m_level_ < m_mazes_.size())
    {
      m_map_.load(m_mazes_[m_level_]);
      ++m_level_;
      jdb::Audio_manager::stop(m_bgm_);
      jdb::Audio_manager::play(m_bgm_);
    }
      else
      {
        jdb::Audio_manager::stop(m_bgm_);
        jdb::Audio_manager::play(m_win_bgm_);
      }
       default:;
    }
  }

}//shadow_maze