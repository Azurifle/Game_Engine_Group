#include "stdafx.h"
#include "Type_data.hpp"
#include "0._Game_Engine/Game_engine.hpp"

namespace G6037599
{
  //___ (de)constructors _____________________________________________
  Type_data::Type_data(const std::string& t_name
    , const std::string& t_dead_message, const std::string& t_atk_name
    , const int t_max_hp, const int t_atk, const int t_max_atk
    , const char t_symbol, const int t_behavior)
    : NAME(t_name), DEAD_MESSAGE(t_dead_message), ATK_NAME(t_atk_name)
      , ATK(t_atk), MAX_ATK(t_max_atk), ORIGIN_MAX_HP(t_max_hp)
      , BEHAVIOR(t_behavior), SYMBOL(t_symbol), m_max_hp_(t_max_hp) {}

  //___ public ________________________________________________
  int Type_data::get_max_hp() const
  {
    return m_max_hp_;
  }

  void Type_data::increase_max_hp(const int t_amount)
  {
    m_max_hp_ += t_amount;
  }

  void Type_data::reset_max_hp()
  {
    m_max_hp_ = ORIGIN_MAX_HP;
  }

  int Type_data::random_atk() const
  {
    return Game_engine::random(ATK, MAX_ATK);
  }
}//G6037599