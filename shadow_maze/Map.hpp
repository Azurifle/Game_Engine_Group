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
    void update(float t_delta_secs);

    //This function will call Renderer::set_projection_orthogonal follow loaded bmp image size.
    //Therefore, watch out if you are going to render anything after this one!
    void render_mini() const;

    Move_state move_player(int t_face);
    jdb::Vec2<float> player_pos_ratio() const;
  private:
    nlohmann::json m_config_{};
    std::vector<std::vector<int>> m_tiles_{}, m_tiles_minimap_{};
    std::vector<std::vector<int>> m_mesh_type_{}, m_anime_{}, m_other_int_{};
    std::vector<jdb::Texture> m_tiles_other_{};

    enum Meshtype { NORMAL, OBJ4_X4, TILESETS };
    std::vector<std::shared_ptr<jdb::Mesh>> m_obj4_x4_mesh_{}, m_tilesets_mesh_{};
    std::shared_ptr<jdb::Mesh> m_tile_mesh_, m_wall_mesh_;

    jdb::Vec3<float> m_player_pos_;
    int m_player_face_;
    jdb::Texture m_grass_texture_{}, m_wall_texture_{}
      , m_player_texture_{}, m_warp_texture_{}

      , m_grass_mini_texture_{}, m_wall_mini_texture_{}
      , m_player_mini_texture_{}, m_warp_mini_texture_{};

    int m_normal_tick_ = 0;
    int m_animation_tick_ = m_normal_tick_;
    int m_dummy_count_ = 0;
    float m_t_ = 0;

    Map(const Map& t_to_copy) = default;
    Map(Map&& t_to_move) noexcept = default;
    Map& operator=(const Map& t_to_copy) = default;
    Map& operator=(Map&& t_to_move) noexcept = default;

    void setup_tiles_textures(const std::vector<std::vector<jdb::Vec3<int>>>& t_bmp);
    void set_tex(unsigned t_row, unsigned t_col, const std::string& t_texture
      , int t_meshtype,int t_index = 0);
	  void set_tex_minimap(unsigned t_row, unsigned t_col, const std::string& t_texture);
	  void set_tex_other(const std::string& t_texture);

	  bool animation(unsigned t_row, unsigned t_col, const std::string& t_texture
      , int t_frameloop,bool t_play_all = false);

    static void swap_player_tile(std::vector<std::vector<int>>& t_tiles
      , const jdb::Vec2<int>& t_new_pos, const jdb::Vec2<int>& t_player_pos);
  };
}//shadow_maze

#endif //SHADOW_MAZE_MAP_HPP