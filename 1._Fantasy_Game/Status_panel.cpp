#include "stdafx.h"
#include "Status_panel.hpp"
#include "Hp_bar.hpp"
#include "Console.hpp"

namespace jdb
{
  //___ (de)constructors _____________________________________________
  Status_panel::Status_panel(COORD t_pos) : m_start_(t_pos), m_is_hide_(true)
  {
    ++t_pos.Y;
    ++t_pos.X;
    m_hp_ = std::make_unique<Hp_bar>(t_pos);
  }

  Status_panel::Status_panel(const Status_panel& t_to_copy) 
    : m_start_(t_to_copy.m_start_)
    , m_hp_(std::make_unique<Hp_bar>(t_to_copy.m_hp_->get_pos()))
    , m_is_hide_(t_to_copy.m_is_hide_) {}

  Status_panel& Status_panel::operator=(const Status_panel& t_to_copy)
  {
    m_start_ = t_to_copy.m_start_;
    m_hp_ = std::make_unique<Hp_bar>(t_to_copy.m_hp_->get_pos());
    m_is_hide_ = t_to_copy.m_is_hide_;
    return *this;
  }

  //___ public _______________________________________________________
  COORD Status_panel::get_pos() const
  {
    return m_start_;
  }

  void Status_panel::show(const std::string& t_name, const int t_hp
    , const int t_max_hp, const int t_atk, const int t_max_atk)
  {
    m_is_hide_ = false;

    auto cursor = m_start_;
    Console::set_cursor(cursor);
    std::cout << "-- " << t_name << " ";
    std::cout << std::setw(cursor.X + WIDTH - Console::get_cursor().X) 
      << std::setfill('-') << '-';

    m_hp_->set(t_hp, t_max_hp);

    const auto ROW_ATK = 2;
    cursor.Y += ROW_ATK;
    Console::set_cursor(cursor);
    std::cout << " ATK: " << t_atk << " - " << t_max_atk;

    ++cursor.Y;
    Console::set_cursor(cursor);
    std::cout << std::setw(WIDTH) << std::setfill('-') << '-';
  }

  void Status_panel::hide()
  {
    switch (m_is_hide_)
    {
      case true: return; 
      default: m_is_hide_ = true;
    }

    for(auto cursor = m_start_; cursor.Y < m_start_.Y+HIEGHT; ++cursor.Y)
    {
      Console::set_cursor(cursor);
      std::cout << std::setw(WIDTH) << std::setfill(' ') << ' ';
    }//for clear Status_panel
  }

  void Status_panel::set(const int t_hp, const int t_max_hp) const
  {
    m_hp_->set(t_hp, t_max_hp);
  }
}//jdb