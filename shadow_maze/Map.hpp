#ifndef SHADOW_MAZE_MAP_HPP
#define SHADOW_MAZE_MAP_HPP
#pragma once

namespace shadow_maze
{
  class Tile;

  class Map final
  {
  public:
    Map() = default;
    ~Map() = default;

    void load(const std::string& t_bmp_path);

    //This function will call Renderer::set_projection_orthogonal follow loaded bmp image size.
    //Therefore, watch out if you are going to render anything after this one!
    void render() const;
  private:
    std::vector<std::vector<Tile>> m_tiles_;

    Map(const Map& t_to_copy) = default;
    Map(Map&& t_to_move) noexcept = default;
    Map& operator=(const Map& t_to_copy) = default;
    Map& operator=(Map&& t_to_move) noexcept = default;

    void setup_terrain(std::vector<std::vector<std::vector<int>>> t_bmp);
  };
}//shadow_maze

#endif //SHADOW_MAZE_MAP_HPP