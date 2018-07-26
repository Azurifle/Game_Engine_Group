#include <stdafx.h>
#include "Map.hpp"
#include <jdb_engine/visual/Renderer.hpp>
#include <jdb_engine/Engine.hpp>
#include "Tile.hpp"

namespace shadow_maze
{
  // ___ public __________________________________________________________________________________

  void Map::load(const std::string& t_bmp_path)
  {
    REQUIRE(!t_bmp_path.empty());

    m_tiles_.clear();

    std::vector<std::vector<std::vector<int>>> bmp;
    jdb::Engine::load_bmp(t_bmp_path, bmp);

    m_tiles_.resize(bmp.size()
      , std::vector<Tile>(bmp[0].size(), Tile())
    );

    setup_terrain(bmp);
  }

  void Map::render() const
  {
    static const jdb::Vec2<float> MAP_SIZE(m_tiles_.size(), m_tiles_[0].size());
    jdb::Renderer::set_projection_orthogonal(MAP_SIZE);

    for (const auto row : m_tiles_)
    {
      for (const auto tile : row)
      {
        tile.render();
      }
    }//row loop
  }

  // ___ private _________________________________________________________________________________

  void Map::setup_terrain(std::vector<std::vector<std::vector<int>>> t_bmp)
  {
    enum Color { B, G, R, WALL_COLOR = 0 };

    for (unsigned row = 0; row < m_tiles_.size(); ++row)
    {
      for (unsigned col = 0; col < m_tiles_[0].size(); ++col)
      {
        if (t_bmp[row][col][B] == WALL_COLOR
          && t_bmp[row][col][G] == WALL_COLOR
          && t_bmp[row][col][R] == WALL_COLOR)
        {
          m_tiles_[row][col].terrain = Tile::Terrain::WALL;
        }
      }//col loop
    }//row loop
  }

}//jdb