#include <stdafx.h>
#include "Game.hpp"
#include <jdb_engine/Gui_system.hpp>
#include <jdb_engine/visual/Mesh_factory.hpp>
#include <jdb_engine/visual/Renderer.hpp>

namespace shadow_maze
{
  // ___ destructor _____________________________________________________________________

  Game::~Game()
  {
    jdb::Audio_manager::stop(m_bgm_);
  }

  // ___ setup glfw window _____________________________________________________________________

  std::string Game::title() { return "Shadow Maze"; }

  void Game::window_size(jdb::Vec2<int>& t_out) { t_out.x = WIDTH; t_out.y = HEIGHT;}

  void Game::key_callback(GLFWwindow* t_window, const int t_key, const int t_scancode
    , const int t_action, const int t_mods)
  {
    if (t_key == GLFW_KEY_F3 && t_action == GLFW_PRESS)
    {
      puts("F3 pressed: Show GLFW window information");
      std::cout << "window:" << t_window << ", scancode: " << t_scancode 
        << ", mods: " << t_mods << std::endl;
	  std::cout << "WIDTH:" << WIDTH << ", HEIGHT: " << HEIGHT << std::endl;

    }
  }

  // ___ public game play ________________________________________________________________________

  void Game::start()
  {
    m_bgm_ = jdb::Audio_manager::load_or_get_audio("shadow_maze/music/Game.wav");
    jdb::Audio_manager::play(m_bgm_);

	load_texture("Grass", GL_RGB);
    load_texture("Test", GL_RGBA);
    load_texture("Smiley", GL_RGBA);
    load_texture("Warp2", GL_RGBA);

    setup_meshes();
  }

  void Game::update()
  {
    static const jdb::Vec3<float> BLACK(0);
    jdb::Renderer::render_bg(BLACK);

	//test mini map
    jdb::Renderer::set_draw_frame(jdb::Vec2<int>(0)
      , jdb::Vec2<int>(static_cast<int>(round(WIDTH * 0.25))
      , static_cast<int>(round(HEIGHT * 0.25))));
    update_game_objects();
	//end mini map test

    jdb::Renderer::set_draw_frame(jdb::Vec2<int>(0), jdb::Vec2<int>(WIDTH, HEIGHT)); 
    update_game_objects();

  }

  //___ private __________________________________________________________________________________

  const std::string Game::TEXTURE_FOLDER("shadow_maze/texture/");

  void Game::load_texture(const std::string& t_png_name, const GLenum t_color_format)
  {
    m_textures_.push_back(jdb::Texture_manager::load_or_get(
      TEXTURE_FOLDER + t_png_name + ".png", t_color_format));
  }

  void Game::setup_meshes()
  {
    const auto TEX_LEFT_TOP = jdb::Vec2<float>(0, 0), TEX_RIGHT_TOP = jdb::Vec2<float>(1, 0)
      , TEX_RIGHT_BOTTOM = jdb::Vec2<float>(1, 1), TEX_LEFT_BOTTOM = jdb::Vec2<float>(0, 1);

	const float DY_FOUR = 0.25f;
	//
    jdb::Mesh_factory mesh_factory{};
    mesh_factory.new_mesh(GL_TRIANGLES, jdb::Shader_manager::load_or_get());
    mesh_factory.add_vertex(jdb::Vec3<float>(6, -4), TEX_RIGHT_BOTTOM);
    mesh_factory.add_vertex(jdb::Vec3<float>(-6, -4), TEX_LEFT_BOTTOM);
    mesh_factory.add_vertex(jdb::Vec3<float>(0, 6), jdb::Vec2<float>(0.5f, 0));
    m_test_mesh_ = mesh_factory.save_mesh();

    mesh_factory.new_mesh(GL_QUADS);
    static const auto TILE_SIZE = 5.0f;
    mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), TEX_LEFT_TOP);
    mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), TEX_RIGHT_TOP);
    mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), TEX_RIGHT_BOTTOM);
    mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), TEX_LEFT_BOTTOM);
    m_tile_mesh_ = mesh_factory.save_mesh();

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

  float AnimeData = 0;
  int DummyTime = 0;
  void Game::update_game_objects() const
  {
    enum Texture_id { GRASS,TEST , SMILEY,HERO};
	DummyTime++;
	if (DummyTime >= 1000)
	{
		AnimeData++;
		if (AnimeData > 15)
		{
			AnimeData = 0;
		}
		DummyTime = 0;
	}


	jdb::Renderer::push_matrix();
	  //jdb::Renderer::scale(jdb::Vec3<float>(5, 5, 0));
	  //jdb::Renderer::translate(jdb::Vec3<float>(0, 0, 0));
	  jdb::Renderer::use_texture(m_textures_[GRASS]);
	  jdb::Renderer::draw_mesh(m_tile_mesh_);
	jdb::Renderer::pop_matrix();

    jdb::Renderer::push_matrix();
      jdb::Renderer::translate(jdb::Vec3<float>(-10, 0, 0));
      jdb::Renderer::use_texture(m_textures_[GRASS]);
      jdb::Renderer::draw_mesh(m_tile_mesh_);
    jdb::Renderer::pop_matrix();

	jdb::Renderer::push_matrix();
	  //jdb::Renderer::scale(jdb::Vec3<float>(2, 2, 0));
	  jdb::Renderer::use_texture(m_textures_[HERO]);
	  jdb::Renderer::draw_mesh(m_player_mesh_[AnimeData]);
    jdb::Renderer::pop_matrix();

  }

}//shadow_maze