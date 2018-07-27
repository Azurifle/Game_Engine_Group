#ifndef SHADOW_MAZE_MAP_HPP
#define SHADOW_MAZE_MAP_HPP
#pragma once
#include <jdb_engine/visual/Texture_manager.hpp>
#include <jdb_engine/my_math/Vec3.hpp>
#include <jdb_engine/thirdparty/json.hpp>
#include "jdb_engine/visual/Mesh.hpp"

namespace shadow_maze
{
  class Map final
  {
  public:
    Map();
    ~Map() = default;

    //This function must be call after Gui_system::create_window(...)
    void load(const std::string& t_bmp_path);
    //This function must be call after Gui_system::create_window(...)
	void init(const std::string& t_bmp_path);

    //This function will call Renderer::set_projection_orthogonal follow loaded bmp image size.
    //Therefore, watch out if you are going to render anything after this one!
	void update(float t_delta_secs);
	void render_mini();
  private:
    nlohmann::json m_config_{};
	std::vector<std::vector<jdb::Texture>> m_tiles_, m_tiles_minimap;

	std::vector<jdb::Texture> m_tiles_other_;
	std::vector<std::vector<int>> m_mesh_type_;
	std::vector<std::vector<int>> m_anime_;

	std::vector<std::vector<int>> m_other_int_;

    std::shared_ptr<jdb::Mesh> m_tile_mesh_ , m_wall_mesh_;
	std::vector<std::shared_ptr<jdb::Mesh>> m_obj4x4_mesh_, m_tilesets_mesh_;

    Map(const Map& t_to_copy) = default;
    Map(Map&& t_to_move) noexcept = default;
    Map& operator=(const Map& t_to_copy) = default;
    Map& operator=(Map&& t_to_move) noexcept = default;

    void setup_tiles_textures(const std::vector<std::vector<jdb::Vec3<int>>>& t_bmp);
    void set_tex(unsigned t_row, unsigned t_col, const std::string& t_texture ,const int t_meshtype,int t_index = 0);
	void set_tex_minimap(unsigned t_row, unsigned t_col, const std::string& t_texture);
	void set_tex_other(const std::string& t_texture);
	
	bool animation(unsigned t_row, unsigned t_col, const std::string& t_texture,int t_frameloop,bool t_playAll = false);
	

	enum Meshtype { NORMAL, OBJ4X4, TILESETS};
	int normal_tick = 0;
	int animation_tick = normal_tick;
	int dummycount = 0;
	float t = 0;
  };
}//shadow_maze

#endif //SHADOW_MAZE_MAP_HPP