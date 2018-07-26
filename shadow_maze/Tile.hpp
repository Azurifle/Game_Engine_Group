#ifndef SHADOW_MAZE_TILE_HPP
#define SHADOW_MAZE_TILE_HPP
#pragma once

namespace shadow_maze
{
  class Tile final
  {
  public:
    enum Terrain { WALL, GRASS };
    enum Visual { NO, YES, FOGGED };
    enum Owner_state {NONE = -1};

    Terrain terrain;
    Visual visual;
    int owner_id;

    Tile();
    void render() const;
  };
}//shadow_maze

#endif //SHADOW_MAZE_TILE_HPP