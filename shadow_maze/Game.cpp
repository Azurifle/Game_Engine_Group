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
  , m_anime_data_(0), m_dummy_time_(0) {}

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

    m_grass_tile_.texture = load_texture("Grass");
    m_a_texture_ = load_texture("Warp2", GL_RGBA);

    jdb::Renderer::set_projection_orthogonal(jdb::Vec2<int>(20));
    setup_meshes();
  }

  void Game::update()
  {
    static const jdb::Vec3<float> BLACK(0);
    jdb::Renderer::render_bg(BLACK);

    m_grass_tile_.render();

    m_dummy_time_++;
    if (m_dummy_time_ >= 1000)
    {
      m_anime_data_++;
      if (m_anime_data_ > 15)
      {
        m_anime_data_ = 0;
      }
      m_dummy_time_ = 0;
    }

    jdb::Renderer::push_matrix();
      jdb::Renderer::use_texture(m_a_texture_);
      jdb::Renderer::draw_mesh(m_player_mesh_[m_anime_data_]);
    jdb::Renderer::pop_matrix();
  }

  //___ private __________________________________________________________________________________

  jdb::Texture Game::load_texture(const std::string& t_png_name, const GLenum t_color_format)
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
    static const auto TILE_SIZE = 1.0f;
    mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), TEX_LEFT_TOP);
    mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), TEX_RIGHT_TOP);
    mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), TEX_RIGHT_BOTTOM);
    mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), TEX_LEFT_BOTTOM);
    m_grass_tile_.mesh = mesh_factory.save_mesh();

    static const auto DY_FOUR = 0.25f;

	  //0 Down
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(0, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(0, DY_FOUR));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //1 Down
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR*2, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR*2, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //2 Down
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR*2, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR*3, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR*3, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR*2, DY_FOUR));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //3 Down
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, 0));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //4 Left
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(0, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(0, DY_FOUR*2));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //5 Left
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR*2));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //6 Left
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR*2));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //7 Left
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, DY_FOUR));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR*2));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //8 Right
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(0, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(0, DY_FOUR * 3));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //9 Right
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR * 3));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //10 Right
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR * 3));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //11 Right
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, DY_FOUR*2));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR * 3));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //12 Up
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(0, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR * 4));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(0, DY_FOUR * 4));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //13 Up
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR * 4));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR, DY_FOUR * 4));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //14 Up
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR * 4));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 2, DY_FOUR * 4));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());

	  //15 Up
	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, DY_FOUR * 3));
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 4, DY_FOUR * 4));
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), jdb::Vec2<float>(DY_FOUR * 3, DY_FOUR * 4));
	  m_player_mesh_.push_back(mesh_factory.save_mesh());
  }

}//shadow_maze