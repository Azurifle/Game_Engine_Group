#include "stdafx.h"
#include "Tile_data.hpp"

namespace jdb
{
  //___ (de)constructors _____________________________________________
  Tile_data::Tile_data() : ID(m_id_count_), m_pos_({ 0, 0 })
  {
    ++m_id_count_;
  }

  Tile_data::Tile_data(const COORD& t_pos) : ID(m_id_count_), m_pos_(t_pos)
  {
    ++m_id_count_;
  }

  //___ public ________________________________________________
  COORD Tile_data::get_pos() const
  {
    return m_pos_;
  }

  void Tile_data::set_pos(const COORD& t_pos)
  {
    m_pos_ = t_pos;
  }

  //___ private static _____________________________________________
  int Tile_data::m_id_count_ = 0;
}//jdb