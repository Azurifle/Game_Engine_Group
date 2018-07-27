#include <stdafx.h>
#include "Map.hpp"
#include <jdb_engine/Engine.hpp>
#include <jdb_engine/my_math/Vec2.hpp>
#include <jdb_engine/visual/Renderer.hpp>
#include <jdb_engine/visual/Mesh_factory.hpp>

namespace shadow_maze
{
  // ___ public __________________________________________________________________________________

  Map::Map(): m_config_(jdb::Engine::load_json("shadow_maze/maze/Map_reading.json"))
    , m_player_face_(DOWN) {}

  void Map::init(const std::string& t_bmp_path)
  {
    REQUIRE(jdb::Engine::state() >= jdb::Engine::State::RUNNING);

    const auto TEX_LEFT_TOP = jdb::Vec2<float>(0, 0), TEX_RIGHT_TOP = jdb::Vec2<float>(1, 0)
      , TEX_RIGHT_BOTTOM = jdb::Vec2<float>(1, 1), TEX_LEFT_BOTTOM = jdb::Vec2<float>(0, 1);

    jdb::Mesh_factory mesh_factory(GL_QUADS);
    const auto TILE_SIZE = 0.5f;
    mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, TILE_SIZE), TEX_LEFT_TOP);
    mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, TILE_SIZE), TEX_RIGHT_TOP);
    mesh_factory.add_vertex(jdb::Vec3<float>(TILE_SIZE, -TILE_SIZE), TEX_RIGHT_BOTTOM);
    mesh_factory.add_vertex(jdb::Vec3<float>(-TILE_SIZE, -TILE_SIZE), TEX_LEFT_BOTTOM);
    m_tile_mesh_ = mesh_factory.save_mesh();

    m_grass_texture_ = jdb::Texture_manager::load_or_get(m_config_["grass_texture"]);
    m_wall_texture_ = jdb::Texture_manager::load_or_get(m_config_["wall_texture"]);
    m_player_texture_ = jdb::Texture_manager::load_or_get(m_config_["player_texture"]);
    m_warp_texture_ = jdb::Texture_manager::load_or_get(m_config_["warp_texture"]);

    load(t_bmp_path);
  }

  void Map::load(const std::string& t_bmp_path)
  {
    REQUIRE(jdb::Engine::state() >= jdb::Engine::State::RUNNING);
    REQUIRE(!t_bmp_path.empty());

    m_tiles_.clear();

    std::vector<std::vector<jdb::Vec3<int>>> bmp;
    jdb::Engine::load_bmp(t_bmp_path, bmp);

    m_tiles_.resize(bmp.size()+2, std::vector<jdb::Texture>(bmp[0].size() + 2));

    setup_tiles_textures(bmp);
  }

  void Map::render_mini() const
  {
    static const jdb::Vec2<float> MAP_SIZE(
      static_cast<float>(m_tiles_.size()), static_cast<float>(m_tiles_[0].size()))
      , TO_POS_ON_ORTHO = (MAP_SIZE - 1.0f) / 2.0f;
    jdb::Renderer::set_projection_orthogonal(MAP_SIZE);

    for (auto row = MAP_SIZE.x - 1; row >= 0; --row)
    {
      for (float col = 0; col < MAP_SIZE.y; ++col)
      {
        jdb::Renderer::push_matrix();
        jdb::Renderer::translate(jdb::Vec3<float>(col, row) - TO_POS_ON_ORTHO);
        jdb::Renderer::use_texture(m_grass_texture_);
        jdb::Renderer::draw_mesh(m_tile_mesh_);
        jdb::Renderer::pop_matrix();
      }
    }

    for (auto row = MAP_SIZE.x-1; row >= 0; --row)
    {
      for (float col = 0; col < MAP_SIZE.y; ++col)
      {
        const auto row_int = static_cast<int>(row), col_int = static_cast<int>(col);

        if(m_tiles_[row_int][col_int] != m_grass_texture_)
        {
          jdb::Renderer::push_matrix();
            if(m_tiles_[row_int][col_int] == m_player_texture_)
            {
              jdb::Renderer::scale(jdb::Vec3<float>(0.75f));
              jdb::Renderer::translate(m_player_pos_ - TO_POS_ON_ORTHO);
            }
            else
            {
              jdb::Renderer::translate(jdb::Vec3<float>(col, row) - TO_POS_ON_ORTHO);
            }
          
            jdb::Renderer::use_texture(m_tiles_[row_int][col_int]);
            jdb::Renderer::draw_mesh(m_tile_mesh_);
          jdb::Renderer::pop_matrix();
        }
      }
    }//row loop
  }

  Map::Move_state Map::move_player(const int t_face)
  {
    REQUIRE(!m_tiles_.empty());

    m_player_face_ = t_face;
    auto new_pos = m_player_pos_;
    static const float SPEED = m_config_["player_speed"];
    switch (m_player_face_)
    {
    case DOWN: new_pos += jdb::Vec3<float>(0, -SPEED); break;
    case LEFT: new_pos += jdb::Vec3<float>(-SPEED, 0); break;
    case RIGHT: new_pos += jdb::Vec3<float>(SPEED, 0); break;
    case UP: new_pos += jdb::Vec3<float>(0, SPEED); default:;
    }

    const auto new_pos_y = static_cast<int>(round(new_pos.y))
      , new_pos_x = static_cast<int>(round(new_pos.x))
      , player_pos_y = static_cast<int>(round(m_player_pos_.y))
      , player_pos_x = static_cast<int>(round(m_player_pos_.x));
    static const float PLAYER_RADIUS = m_config_["player_radius"];
    if (m_tiles_[new_pos_y][static_cast<int>(round(new_pos.x+PLAYER_RADIUS))]
      == m_wall_texture_
      || m_tiles_[new_pos_y][static_cast<int>(round(new_pos.x-PLAYER_RADIUS))]
      == m_wall_texture_)
    {
      return NO;
    }

    if(m_tiles_[new_pos_y][new_pos_x] == m_warp_texture_)
    {
      return WARP;
    }
    
    const auto player_texture = m_tiles_[player_pos_y][player_pos_x];
    m_tiles_[player_pos_y][player_pos_x] = m_tiles_[new_pos_y][new_pos_x];
    m_tiles_[new_pos_y][new_pos_x] = player_texture;
    m_player_pos_ = new_pos;

    return YES;
  }

  jdb::Vec2<float> Map::player_pos_ratio() const
  {
    return { m_player_pos_.x / static_cast<float>(m_tiles_[0].size())
      , m_player_pos_.y / static_cast<float>(m_tiles_.size()) };
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
        if (row <= 0 || col <= 0 || row >= m_tiles_.size() - 1 || col >= m_tiles_[0].size() - 1)
          set_tex_wall(row, col);
        else if (t_bmp[row-1][col-1] == BGR_GRASS) m_tiles_[row][col] = m_grass_texture_;
        else if (t_bmp[row - 1][col - 1] == BGR_PLAYER)
        {
          m_tiles_[row][col] = m_player_texture_;
          m_player_pos_ = { static_cast<float>(col), static_cast<float>(row) };
        }
        else if (t_bmp[row-1][col-1] == BGR_WARP) m_tiles_[row][col] = m_warp_texture_;
        else set_tex_wall(row, col);
      }
    }//row loop
  }

  void Map::set_tex_wall(const unsigned t_row, const unsigned t_col)
  {
    m_tiles_[t_row][t_col] = m_wall_texture_;
  }

}//jdb