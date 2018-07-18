#include "stdafx.h"
#include "Map.hpp"

namespace G6037599
{
  //___ (de)constructors _____________________________________________
  Map::Map()
  {
    const auto CAN_CONTAIN_PLAYER_MONSTER_N_SPAWNPOINT = 2;
    REQUIRE(Map::SIZE >= CAN_CONTAIN_PLAYER_MONSTER_N_SPAWNPOINT);

    for(auto row = 0; row < SIZE; ++row)
    {
      m_grid_.push_back(std::make_unique< std::vector<Tile> >(SIZE, Tile{}));
    }

    PROMISE(m_grid_[0]->at(0).owner_id == NO_UNIT);
  }

  Map::Map(const Map& t_to_copy)
  {
    copy_from(t_to_copy);
  }

  Map& Map::operator=(const Map& t_to_copy)
  {
    m_grid_.clear();
    copy_from(t_to_copy);
    return *this;
  }

  //___ public _______________________________________________________
  void Map::marked(const COORD& t_pos, const int t_id)
  {
    REQUIRE(0 <= t_pos.X && t_pos.X < SIZE);
    REQUIRE(0 <= t_pos.Y && t_pos.Y < SIZE);
    REQUIRE(t_id >= NO_UNIT);

    m_grid_[t_pos.Y]->at(t_pos.X).owner_id = t_id;
  }

  bool Map::is_attacker(const COORD& t_pos, const int t_id) const
  {
    REQUIRE(0 <= t_pos.X && t_pos.X < SIZE);
    REQUIRE(0 <= t_pos.Y && t_pos.Y < SIZE);
    REQUIRE(t_id >= NO_UNIT);

    return m_grid_[t_pos.Y]->at(t_pos.X).attacker_id == t_id;
  }

  void Map::move(const COORD& t_from, const int t_id, const COORD& t_to)
  {
    REQUIRE(0 <= t_from.X && t_from.X < SIZE);
    REQUIRE(0 <= t_from.Y && t_from.Y < SIZE);
    REQUIRE(t_id > NO_UNIT);
    REQUIRE(0 <= t_to.X && t_to.X < SIZE);
    REQUIRE(0 <= t_to.Y && t_to.Y < SIZE);

    switch (is_attacker(t_from, t_id))
    {
    case false: PROMISE(m_grid_[t_from.Y]->at(t_from.X).owner_id == t_id);
      m_grid_[t_from.Y]->at(t_from.X).owner_id = NO_UNIT; break;
    default: PROMISE(m_grid_[t_from.Y]->at(t_from.X).attacker_id == t_id);
      m_grid_[t_from.Y]->at(t_from.X).attacker_id = NO_UNIT;
    }

    switch (m_grid_[t_to.Y]->at(t_to.X).owner_id)
    {
    case NO_UNIT: m_grid_[t_to.Y]->at(t_to.X).owner_id = t_id; break;
    default: m_grid_[t_to.Y]->at(t_to.X).attacker_id = t_id;
    }
  }

  int Map::get(const COORD& t_pos) const
  {
    REQUIRE(0 <= t_pos.X && t_pos.X < SIZE);
    REQUIRE(0 <= t_pos.Y && t_pos.Y < SIZE);

    return m_grid_[t_pos.Y]->at(t_pos.X).owner_id;
  }

  COORD Map::random_unoccupied() const
  {
    COORD pos;
    do
    {
      pos.Y = static_cast<short>( rand() % m_grid_.size() );
      pos.X = static_cast<short>( rand() % m_grid_[0]->size() );
    } while (m_grid_[pos.Y]->at(pos.X).owner_id != NO_UNIT || 
      m_grid_[pos.Y]->at(pos.X).attacker_id != NO_UNIT);

    return pos;
  }

  void Map::reset()
  {
    for(auto it_y = m_grid_.begin(); it_y < m_grid_.end(); ++it_y)
    {
      for (auto it_x = (*it_y)->begin(); it_x < (*it_y)->end(); ++it_x)
      {
        (*it_x).owner_id = NO_UNIT;
        (*it_x).attacker_id = NO_UNIT;
      }
    }//for iterator y
  }

  //___ private _______________________________________________________
  void Map::copy_from(const Map& t_to_copy)
  {
    for (auto row = 0; row < t_to_copy.SIZE; ++row)
    {
      m_grid_.push_back(std::make_unique< std::vector<Tile> >(SIZE, Tile{}));
    }
  }

}//G6037599