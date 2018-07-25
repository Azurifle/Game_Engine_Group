#include <stdafx.h>
#include "Game.hpp"
#include <jdb_engine/Gui_system.hpp>
#include <jdb_engine/visual/Mesh_factory.hpp>
#include <jdb_engine/visual/Renderer.hpp>

namespace shadow_maze
{
  // ___ public _____________________________________________________________________

  //How to use json: https://github.com/nlohmann/json/tree/master

  Game::Game(): m_config_(nlohmann::json::parse(std::ifstream("shadow_maze/Config.json")))
  , m_window_size_(jdb::Vec2<int>(m_config_["window_width"], m_config_["window_height"]))
  , m_dummy_time_(0), m_anime_data_(0), anime_data_tile_(0) {}

  Game::~Game()
  {
    jdb::Audio_manager::stop(m_bgm_);
  }

  std::string Game::title() { return "Shadow Maze"; }

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
    m_bgm_ = jdb::Audio_manager::load_or_get_audio("shadow_maze/music/Game.wav");
    jdb::Audio_manager::play(m_bgm_);

    m_warp_tile_.texture = load_texture("Warp2", GL_RGBA);
    m_grass_tile_.texture = load_texture("Tile01", GL_RGBA);

    jdb::Renderer::set_projection_orthogonal(jdb::Vec2<int>(20));
    setup_meshes();
  }

  void Game::update(const float t_delta_secs)
  {
    static const jdb::Vec3<float> BLACK(0);
    jdb::Renderer::render_bg(BLACK);

    m_dummy_time_ += t_delta_secs;
    static const auto HALF_SEC = 0.5f;
    if (m_dummy_time_ >= HALF_SEC)
    {
      m_dummy_time_ = 0;
      m_anime_data_ = (m_anime_data_ + 1)%16;
      anime_data_tile_ = (anime_data_tile_ + 1)%22;
      enum Unuse_tile { TREE_STUMP = 4, STONE = 8, TREE_STUMP2 = 12};
      switch (anime_data_tile_)
      {
      case TREE_STUMP:; case STONE:; case TREE_STUMP2: anime_data_tile_++; break; default:;
      }
    }

    m_grass_tile_.pos.x = 0;
    m_grass_tile_.mesh = m_grass_mesh_[0];
    m_grass_tile_.render();

    m_grass_tile_.pos.x = -1;
    m_grass_tile_.mesh = m_grass_mesh_[0];
    m_grass_tile_.render();

    m_grass_tile_.mesh = m_grass_mesh_[anime_data_tile_];
    m_grass_tile_.render();

    m_grass_tile_.mesh = m_grass_mesh_[anime_data_tile_];
    m_grass_tile_.render();

    m_warp_tile_.mesh = m_player_mesh_[m_anime_data_];
    m_warp_tile_.render();
  }

  //___ private __________________________________________________________________________________

  jdb::Texture Game::load_texture(const std::string& t_png_name
    , const GLenum t_color_format)
  {
    return jdb::Texture_manager::load_or_get(
      m_config_["texture_folder"].get<std::string>() + t_png_name + ".png", t_color_format);
  }

  void Game::setup_meshes()
  {
    const auto TEX_LEFT_TOP = jdb::Vec2<float>(0, 0), TEX_RIGHT_TOP = jdb::Vec2<float>(1, 0)
      , TEX_RIGHT_BOTTOM = jdb::Vec2<float>(1, 1), TEX_LEFT_BOTTOM = jdb::Vec2<float>(0, 1);

    jdb::Mesh_factory mesh_factory{};
    mesh_factory.new_mesh(GL_QUADS);
    static const auto TILE_SIZE = 0.5f
      , DY_FOUR = 1 / 4.0f, DY_TILE_X = 1 / 8.0f, DY_TILE_Y = 1 / 25.0f;

	  //Character
	  for (auto row = 0; row < 4; ++row)
		  for (auto col = 0; col < 4; ++col)
		  {
			  mesh_factory.new_mesh(GL_QUADS);
			  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE)
          , jdb::Vec2<float>(DY_FOUR*(0 + col), DY_FOUR * (0 + row)));
			  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE)
          , jdb::Vec2<float>(DY_FOUR*(1 + col), DY_FOUR * (0 + row)));
			  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE)
          , jdb::Vec2<float>(DY_FOUR*(1 + col), DY_FOUR* (1 + row)));
			  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE)
          , jdb::Vec2<float>(DY_FOUR*(0 + col), DY_FOUR* (1 + row)));
			  m_player_mesh_.push_back(mesh_factory.save_mesh());
		  }

    //Tilesets
	  for (auto row = 0; row < 3; row++)
		  for (auto col = 0; col < 8; col++)
		  {
			  mesh_factory.new_mesh(GL_QUADS);
			  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE)
          , jdb::Vec2<float>(DY_TILE_X*(0 + col), DY_TILE_Y * (0 + row)));
			  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE)
          , jdb::Vec2<float>(DY_TILE_X*(1 + col), DY_TILE_Y * (0 + row)));
			  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE)
          , jdb::Vec2<float>(DY_TILE_X*(1 + col), DY_TILE_Y* (1 + row)));
			  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE)
          , jdb::Vec2<float>(DY_TILE_X*(0 + col), DY_TILE_Y* (1 + row)));
        m_grass_mesh_.push_back(mesh_factory.save_mesh());
		  }
  }

}//shadow_maze