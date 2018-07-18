#include "stdafx.h"
#include "Spawn_point.hpp"
#include "Tile_data.hpp"
#include "Type_data.hpp"
#include "Console.hpp"
#include "Map.hpp"

namespace G6037599
{
  //___ (de)constructors _____________________________________________
  Spawn_point::Spawn_point(const std::shared_ptr<Type_data> t_type
    , const std::shared_ptr<Console> t_console, const std::shared_ptr<Map> t_map)
    : m_type_(t_type), m_console_(t_console), m_map_(t_map)
      , m_tile_(std::make_unique<Tile_data>()) {}

  Spawn_point::Spawn_point(const Spawn_point& t_to_copy) : m_type_(t_to_copy.m_type_)
    , m_console_(t_to_copy.m_console_), m_map_(t_to_copy.m_map_)
    , m_tile_(std::make_unique<Tile_data>(t_to_copy.m_tile_->get_pos())) {}

  Spawn_point& Spawn_point::operator=(const Spawn_point& t_to_copy)
  {
    m_type_ = t_to_copy.m_type_;
    m_console_ = t_to_copy.m_console_;
    m_map_ = t_to_copy.m_map_;
    m_tile_ = std::make_unique<Tile_data>(t_to_copy.m_tile_->get_pos());
    return *this;
  }

  //___ public _____________________________________________
  COORD Spawn_point::get_pos() const
  {
    return m_tile_->get_pos();
  }

  int Spawn_point::get_id() const
  {
    return m_tile_->ID;
  }

  std::string Spawn_point::get_type_name() const
  {
    return m_type_->NAME;
  }

  int Spawn_point::get_type_max_hp() const
  {
    return m_type_->get_max_hp();
  }

  int Spawn_point::get_type_atk() const
  {
    return m_type_->ATK;
  }

  int Spawn_point::get_type_max_atk() const
  {
    return m_type_->MAX_ATK;
  }

  int Spawn_point::get_type_behavior() const
  {
    return m_type_->BEHAVIOR;
  }

  std::string Spawn_point::get_symbol() const
  {
    return std::string("X") + m_type_->SYMBOL;
  }

  std::shared_ptr<Type_data> Spawn_point::share_type() const
  {
    return m_type_;
  }

  void Spawn_point::set_pos(const COORD& t_pos) const
  {
    m_tile_->set_pos(t_pos);
  }

  int Spawn_point::random_type_atk() const
  {
    return m_type_->random_atk();
  }

  void Spawn_point::reset_type() const
  {
    m_type_->reset_max_hp();
  }

  //___ protected _____________________________________________
  void Spawn_point::set_console_monster_hp(const int t_hp, const int t_max_hp) const
  {
    m_console_->set_monster_hp(t_hp, t_max_hp);
  }

  COORD Spawn_point::random_unoccupied_tile() const
  {
    return m_map_->random_unoccupied();
  }

  void Spawn_point::map_move(const COORD& t_from, const int t_id, const COORD& t_to) const
  {
    m_map_->move(t_from, t_id, t_to);
  }

  void Spawn_point::mark_map(const COORD& t_pos, const int t_id) const
  {
    m_map_->marked(t_pos, t_id);
  }

  void Spawn_point::mark_console(const COORD& t_pos, const bool t_is_remove) const
  {
    switch (t_is_remove)
    {
    case false: m_console_->marked(t_pos, std::string(1, m_type_->SYMBOL).c_str());
      break;
    default: m_console_->marked(t_pos, " ");
    }    
  }

  void Spawn_point::increase_type_max_hp(const int t_amount) const
  {
    m_type_->increase_max_hp(t_amount);
  }
}//G6037599