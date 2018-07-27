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

  void Map::load(const std::string& t_bmp_path)
  {
    REQUIRE(!t_bmp_path.empty());

    m_tiles_.clear();

    std::vector<std::vector<jdb::Vec3<int>>> bmp;
    jdb::Engine::load_bmp(t_bmp_path, bmp);

    m_tiles_.resize(bmp.size()+2, std::vector<jdb::Texture>(bmp[0].size() + 2));

    setup_tiles_textures(bmp);
  }

  void Map::pre_render()
  {
    const auto TEX_LEFT_TOP = jdb::Vec2<float>(0, 0), TEX_RIGHT_TOP = jdb::Vec2<float>(1, 0)
      , TEX_RIGHT_BOTTOM = jdb::Vec2<float>(1, 1), TEX_LEFT_BOTTOM = jdb::Vec2<float>(0, 1);

    jdb::Mesh_factory mesh_factory(GL_QUADS);
      const auto TILE_SIZE = 0.5f;
      mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), TEX_LEFT_TOP);
      mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), TEX_RIGHT_TOP);
      mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), TEX_RIGHT_BOTTOM);
      mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), TEX_LEFT_BOTTOM);
    m_tile_mesh_ = mesh_factory.save_mesh();
  }

  void Map::render() const
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

          jdb::Renderer::use_texture(m_tiles_[row][col]);
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

    for (unsigned row = 0; row < m_tiles_.size(); ++row)
    {
      for (unsigned col = 0; col < m_tiles_[0].size(); ++col)
      {
        if(row <= 0 || col <= 0 || row >= m_tiles_.size()-1 || col >= m_tiles_[0].size()-1)
          set_tex(row, col, "wall_texture");
        else if (t_bmp[row-1][col-1] == BGR_GRASS) set_tex(row, col, "grass_texture");
        else if(t_bmp[row-1][col-1] == BGR_PLAYER) set_tex(row, col, "player_texture");
        else if (t_bmp[row-1][col-1] == BGR_WARP) set_tex(row, col, "warp_texture");
        else set_tex(row, col, "wall_texture");
      }
    }//row loop
  }

  void Map::set_tex(const unsigned t_row, const unsigned t_col
    , const std::string& t_texture)
  {
    m_tiles_[t_row][t_col] = jdb::Texture_manager::load_or_get(m_config_[t_texture]);
  }

}//jdb