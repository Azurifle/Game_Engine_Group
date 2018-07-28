#include <stdafx.h>
#include "Map.hpp"
#include <jdb_engine/Engine.hpp>
#include <jdb_engine/my_math/Vec2.hpp>
#include <jdb_engine/visual/Renderer.hpp>
#include <jdb_engine/visual/Mesh_factory.hpp>

namespace shadow_maze
{
  // ___ public __________________________________________________________________________________

  Map::Map(): m_config_(jdb::Engine::load_json("shadow_maze/maze/Map_reading.json")) {}

  void Map::init(const std::string& t_bmp_path)
  {
	  const auto TEX_LEFT_TOP = jdb::Vec2<float>(0, 0), TEX_RIGHT_TOP = jdb::Vec2<float>(1, 0)
		  , TEX_RIGHT_BOTTOM = jdb::Vec2<float>(1, 1), TEX_LEFT_BOTTOM = jdb::Vec2<float>(0, 1);
	  const auto TILE_SIZE = 0.5f, DY_FOUR = 1 / 4.0f, DY_TILE_X = 1 / 8.0f, DY_TILE_Y = 1 / 25.0f;

	  jdb::Mesh_factory mesh_factory(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), TEX_LEFT_TOP);
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), TEX_RIGHT_TOP);
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), TEX_RIGHT_BOTTOM);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), TEX_LEFT_BOTTOM);
	  m_tile_mesh_ = mesh_factory.save_mesh();

	  mesh_factory.new_mesh(GL_QUADS);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, 1.05f), TEX_LEFT_TOP);
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, 1.05f), TEX_RIGHT_TOP);
	  mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), TEX_RIGHT_BOTTOM);
	  mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), TEX_LEFT_BOTTOM);
	  m_wall_mesh_ = mesh_factory.save_mesh();

	  //Character 4x4
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
			  m_obj4x4_mesh_.push_back(mesh_factory.save_mesh());
		  }

	  //Tilesets 8x...
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
			  m_tilesets_mesh_.push_back(mesh_factory.save_mesh());
		  }

	  load(t_bmp_path);
  }

  void Map::load(const std::string& t_bmp_path)
  {
    REQUIRE(!t_bmp_path.empty());

    m_tiles_.clear();
	m_tiles_minimap.clear();
	m_tiles_other_.clear();

	m_other_int_.clear();

	m_mesh_type_.clear();
	m_anime_.clear();

    std::vector<std::vector<jdb::Vec3<int>>> bmp;
    jdb::Engine::load_bmp(t_bmp_path, bmp);

    m_tiles_.resize(bmp.size()+2, std::vector<jdb::Texture>(bmp[0].size() + 2));
	m_tiles_minimap.resize(bmp.size() + 2, std::vector<jdb::Texture>(bmp[0].size() + 2));

	m_mesh_type_.resize(bmp.size() + 2, std::vector<int>(bmp[0].size() + 2));
	m_anime_.resize(bmp.size() + 2, std::vector<int>(bmp[0].size() + 2));

	m_other_int_.resize(bmp.size() + 2, std::vector<int>(bmp[0].size() + 2));


    setup_tiles_textures(bmp);
  }

  void Map::update(const float t_delta_secs)
  {
	t += t_delta_secs;
	if (t >= 0.25f)
	{
		dummycount++;
		t = 0;
	}

    static const jdb::Vec2<float> MAP_SIZE(
      static_cast<float>(m_tiles_.size()), static_cast<float>(m_tiles_[0].size()))
      , OFFSET = (MAP_SIZE - 1.0f) / 2.0f;
    jdb::Renderer::set_projection_orthogonal(MAP_SIZE);

	for (int row = m_tiles_.size()-1; row >= 0; --row)//for (unsigned row = 0; row < m_tiles_.size(); ++row)
    {
      for (unsigned col = 0; col < m_tiles_[0].size(); ++col)
      {
		//Animation 
		if (jdb::Texture_manager::load_or_get(m_config_["player_texture"]) == m_tiles_[row][col])
		{
			normal_tick = 8; // player dir (Down = 0  left = 4 right = 8 down = 12)
		}
		else
		{
			normal_tick = m_anime_[row][col];
		}
		if (animation(row, col, m_config_["player_texture"],4)) {}
		else if (animation(row, col, m_config_["warp_texture"],4,true)) {}
		else
		{
			animation_tick = normal_tick;
		}
		//End Animation

        jdb::Renderer::push_matrix();
			jdb::Renderer::push_matrix();
				jdb::Renderer::translate(jdb::Vec3<float>(col, row) - OFFSET);
				//BG
				jdb::Renderer::use_texture(jdb::Texture_manager::load_or_get(m_config_["grass_texture"]));
				jdb::Renderer::draw_mesh(m_tilesets_mesh_[m_config_["grass_index"]]);
				//End BG

				//other obj
				jdb::Renderer::push_matrix();
					jdb::Renderer::translate(jdb::Vec3<float>(0, 0.05f, 0));
					switch (m_other_int_[row][col])
					{
					case 0:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[1]);
						break;
					case 1:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[2]);
						break;
					case 2:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[3]);
						break;
					case 3:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[21]);
						break;
					case 4:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[6]);
						break;
					case 5:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[18]);
						break;
					case 6:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[19]);
						break;
					case 7:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[20]);
						break;
					case 8:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[13]);
						break;
					case 9:
						jdb::Renderer::use_texture(m_tiles_other_[1]);
						jdb::Renderer::draw_mesh(m_tilesets_mesh_[14]);
						break;
					default:
						break;
					}
				jdb::Renderer::pop_matrix();
				//End other obj

				//warp obj
				jdb::Renderer::push_matrix();
				if (jdb::Texture_manager::load_or_get(m_config_["warp_texture"]) == m_tiles_[row][col])
				{
					jdb::Renderer::translate(jdb::Vec3<float>(0, 0.35f, 0));
					jdb::Renderer::use_texture(m_tiles_other_[0]);
					jdb::Renderer::draw_mesh(m_obj4x4_mesh_[animation_tick]);
				}
				jdb::Renderer::pop_matrix();
				//End warp obj
			jdb::Renderer::pop_matrix();

		  if (jdb::Texture_manager::load_or_get(m_config_["player_texture"]) == m_tiles_[row][col])
		  {
			  jdb::Renderer::scale(jdb::Vec3<float>(0.7f, 1.2f, 0));
			  jdb::Renderer::translate(jdb::Vec3<float>(0, 0.5f, 0));
		  }
		  if (jdb::Texture_manager::load_or_get(m_config_["warp_texture"]) == m_tiles_[row][col])
		  {
			  jdb::Renderer::translate(jdb::Vec3<float>(0, 0.35f, 0));
			  jdb::Renderer::scale(jdb::Vec3<float>(0.5f, 2.0f, 0));
		  }
		  jdb::Renderer::translate(jdb::Vec3<float>(col, row) - OFFSET);

		  if (jdb::Texture_manager::load_or_get(m_config_["grass_texture"]) != m_tiles_[row][col])
		  {
			  jdb::Renderer::use_texture(m_tiles_[row][col]);
			  switch (m_mesh_type_[row][col])
			  {
			  case Meshtype::NORMAL:
				  jdb::Renderer::draw_mesh(m_wall_mesh_);
				  break;
			  case Meshtype::OBJ4X4:
				  jdb::Renderer::draw_mesh(m_obj4x4_mesh_[animation_tick]);
				  break;
			  case Meshtype::TILESETS:
				  jdb::Renderer::draw_mesh(m_tilesets_mesh_[animation_tick]);
				  break;
			  default:
				  break;
			  }
		  }
        jdb::Renderer::pop_matrix();
      }
    }//row loop
  }

  void Map::render_mini()
  {
	  static const jdb::Vec2<float> MAP_SIZE(
		  static_cast<float>(m_tiles_.size()), static_cast<float>(m_tiles_[0].size()))
		  , OFFSET = (MAP_SIZE - 1.0f) / 2.0f;
	  jdb::Renderer::set_projection_orthogonal(MAP_SIZE);

	  for (unsigned row = 0; row < m_tiles_.size(); ++row)
	  {
		  for (unsigned col = 0; col < m_tiles_[0].size(); ++col)
		  {
			  jdb::Renderer::push_matrix();
				  jdb::Renderer::translate(jdb::Vec3<float>(col, row) - OFFSET);
				  jdb::Renderer::scale(jdb::Vec3<float>(1.05));
				  jdb::Renderer::use_texture(m_tiles_minimap[row][col]);
				  jdb::Renderer::draw_mesh(m_tile_mesh_);
			  jdb::Renderer::pop_matrix();
		  }
	  }//row loop
  }

  // ___ private _________________________________________________________________________________

  void Map::setup_tiles_textures(const std::vector<std::vector<jdb::Vec3<int>>>& t_bmp)
  {
    static const jdb::Vec3<int> BGR_GRASS(
      m_config_["grass.b"], m_config_["grass.g"], m_config_["grass.r"])
      , BGR_PLAYER(m_config_["player.b"], m_config_["player.g"], m_config_["player.r"])
      , BGR_WARP(m_config_["warp.b"], m_config_["warp.g"], m_config_["warp.r"]);

	set_tex_other("warp_texture2"); // 0
	set_tex_other("grass_texture"); // 1



    for (unsigned row = 0; row < m_tiles_.size(); ++row)
    {
      for (unsigned col = 0; col < m_tiles_[0].size(); ++col)
      {
        if(row <= 0 || col <= 0 || row >= m_tiles_.size()-1 || col >= m_tiles_[0].size()-1)
          set_tex(row, col, "wall_texture",Meshtype::NORMAL);
        else if (t_bmp[row-1][col-1] == BGR_GRASS) set_tex(row, col, "grass_texture"
			, m_config_["grass_type"], m_config_["grass_index"]);
        else if(t_bmp[row-1][col-1] == BGR_PLAYER) set_tex(row, col, "player_texture"
			, m_config_["player_type"], m_config_["player_index"]);
		else if (t_bmp[row - 1][col - 1] == BGR_WARP) set_tex(row, col, "warp_texture"
			, m_config_["warp_type"], m_config_["warp_index"]);
        else set_tex(row, col, "wall_texture", Meshtype::NORMAL);
		//for mini map
		if (row <= 0 || col <= 0 || row >= m_tiles_.size() - 1 || col >= m_tiles_[0].size() - 1)
			set_tex_minimap(row, col, "wall_mini_texture");
		else if (t_bmp[row - 1][col - 1] == BGR_GRASS) set_tex_minimap(row, col, "grass_mini_texture");
		else if (t_bmp[row - 1][col - 1] == BGR_PLAYER) set_tex_minimap(row, col, "player_mini_texture");
		else if (t_bmp[row - 1][col - 1] == BGR_WARP) set_tex_minimap(row, col, "warp_mini_texture");
		else set_tex_minimap(row, col, "wall_mini_texture");
		//for other obj
		m_other_int_[row][col] = jdb::Engine::random(0,15);
      }
    }//row loop
  }

  void Map::set_tex(const unsigned t_row, const unsigned t_col
    , const std::string& t_texture , const int t_meshtype, int t_index)
  {
    if(t_meshtype == 0)
		m_tiles_[t_row][t_col] = jdb::Texture_manager::load_or_get(m_config_[t_texture]);
	else
		m_tiles_[t_row][t_col] = jdb::Texture_manager::load_or_get(m_config_[t_texture],GL_RGBA);

	m_mesh_type_[t_row][t_col] = t_meshtype;
	m_anime_[t_row][t_col] = t_index;
  }

  void Map::set_tex_minimap(unsigned t_row, unsigned t_col,const std::string & t_texture)
  {
	m_tiles_minimap[t_row][t_col] = jdb::Texture_manager::load_or_get(m_config_[t_texture], GL_RGBA);
  }

  void Map::set_tex_other(const std::string & t_texture)
  {
	  m_tiles_other_.push_back(jdb::Texture_manager::load_or_get(m_config_[t_texture], GL_RGBA));
  }

  bool Map::animation(unsigned t_row, unsigned t_col, const std::string & t_texture, int t_frameloop, bool t_playAll)
  {
	  REQUIRE(t_frameloop + normal_tick <= m_obj4x4_mesh_.size() && t_frameloop >= 0);
		 
	  if (t_playAll)
	  {
		  t_frameloop = m_obj4x4_mesh_.size();
		  normal_tick = 0;
	  }

	  if (jdb::Texture_manager::load_or_get(t_texture) == m_tiles_[t_row][t_col])
	  {
		  animation_tick = normal_tick + (dummycount % t_frameloop);
		  return true;
	  }
	  else
		  return false;
  }

}//jdb