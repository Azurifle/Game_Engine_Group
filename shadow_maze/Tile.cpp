#include <stdafx.h>
#include "Tile.hpp"

namespace shadow_maze
{
  Tile::Tile(): terrain(GRASS), visual(NO), owner_id(NONE) {}

  void Tile::render() const
  {
  }
}//shadow_maze