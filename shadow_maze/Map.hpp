#ifndef SHADOW_MAZE_MAP_HPP
#define SHADOW_MAZE_MAP_HPP
#pragma once
#include <jdb_engine/visual/Texture_manager.hpp>
#include <jdb_engine/my_math/Vec3.hpp>
#include <jdb_engine/thirdparty/json.hpp>
#include <jdb_engine/visual/Mesh.hpp>

namespace shadow_maze
{
  class Map final
  {
  public:
    enum Face { DOWN, LEFT = 4, RIGHT = 8, UP = 12 };
    enum Move_state {NO, YES, WARP};

    Map();
    ~Map() = default;

    //This function must be call after Gui_system::create_window(...)
    void init(const std::string& t_bmp_path);
    //This function must be call after Gui_system::create_window(...)
    void load(const std::string& t_bmp_path);

    //This function will call Renderer::set_projection_orthogonal follow loaded bmp image size.
    //Therefore, watch out if you are going to render anything after this one!
    void render_mini() const;
    Move_state move_player(int t_face);
    jdb::Vec2<float> player_pos_ratio() const;
  private:
    nlohmann::json m_config_{};
    std::vector<std::vector<jdb::Texture>> m_tiles_;
    std::shared_ptr<jdb::Mesh> m_tile_mesh_;
    jdb::Vec3<float> m_player_pos_;
    int m_player_face_;
    jdb::Texture m_grass_texture_{}, m_wall_texture_{}, m_player_texture_{}, m_warp_texture_{};

    Map(const Map& t_to_copy) = default;
    Map(Map&& t_to_move) noexcept = default;
    Map& operator=(const Map& t_to_copy) = default;
    Map& operator=(Map&& t_to_move) noexcept = default;

    void setup_tiles_textures(const std::vector<std::vector<jdb::Vec3<int>>>& t_bmp);
    void set_tex_wall(unsigned t_row, unsigned t_col);
  };
}//shadow_maze

#endif //SHADOW_MAZE_MAP_HPP