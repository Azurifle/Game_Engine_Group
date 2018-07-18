#include "stdafx.h"
#include "Unit.hpp"
#include "Type_data.hpp"
#include "Tile_data.hpp"
#include "0._Game_Engine/Game_engine.hpp"

namespace G6037599
{
  //___ (de)constructors _____________________________________________
  Unit::Unit(const std::shared_ptr<Type_data> t_type) : m_type_(t_type)
    , m_tile_(std::make_unique<Tile_data>()), m_hp_(t_type->get_max_hp()) {}

  Unit::Unit(const Unit& t_to_copy) : m_type_(t_to_copy.m_type_)//shared
    , m_tile_(std::make_unique<Tile_data>(t_to_copy.m_tile_->get_pos()))
    , m_hp_(t_to_copy.m_hp_) {}

  Unit& Unit::operator=(const Unit& t_to_copy)
  {
    m_hp_ = t_to_copy.m_hp_;
    m_type_ = t_to_copy.m_type_;//shared
    m_tile_ = std::make_unique<Tile_data>(t_to_copy.m_tile_->get_pos());
    return *this;
  }

  //___ public ________________________________________________
  COORD Unit::get_pos() const
  {
    return m_tile_->get_pos();
  }

  int Unit::get_id() const
  {
    return m_tile_->ID;
  }

  char Unit::get_symbol() const
  {
    return m_type_->SYMBOL;
  }

  int Unit::get_hp() const
  {
    return m_hp_;
  }

  void Unit::set_pos(const COORD& t_pos) const
  {
    m_tile_->set_pos(t_pos);
  }

  void Unit::damaged(const int t_damage)
  {
    REQUIRE(t_damage > 0);

    m_hp_ -= t_damage;
    if(m_hp_ < 0)
    {
      m_hp_ = 0;
    }
  }

  void Unit::set_hp(const int t_hp)
  {
    REQUIRE(0 <= t_hp && t_hp <= m_type_->get_max_hp());

    m_hp_ = t_hp;
  }

  int Unit::random_atk() const
  {
    return m_type_->random_atk();
  }
}//G6037599
