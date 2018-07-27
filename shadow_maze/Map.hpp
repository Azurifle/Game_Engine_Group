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
    void pre_render();

    //This function will call Renderer::set_projection_orthogonal follow loaded bmp image size.
    //Therefore, watch out if you are going to render anything after this one!
    void render() const;
  private:
    nlohmann::json m_config_{};
    std::vector<std::vector<jdb::Texture>> m_tiles_;
    std::shared_ptr<jdb::Mesh> m_tile_mesh_;

    Map(const Map& t_to_copy) = default;
    Map(Map&& t_to_move) noexcept = default;
    Map& operator=(const Map& t_to_copy) = default;
    Map& operator=(Map&& t_to_move) noexcept = default;

    void setup_tiles_textures(const std::vector<std::vector<jdb::Vec3<int>>>& t_bmp);
    void set_tex(unsigned t_row, unsigned t_col, const std::string& t_texture);
  };
}//shadow_maze

#endif //SHADOW_MAZE_MAP_HPP