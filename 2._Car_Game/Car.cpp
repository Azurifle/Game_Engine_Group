#include "stdafx.h"
#include "Car.hpp"
#include "0._Game_Engine/Grid.hpp"
#include "0._Game_Engine/Game_engine.hpp"

namespace jdb
{
  //___ constructors _____________________________________________
  Car::Car(const std::string& t_name, const std::string& t_shape
    , const float t_speed, const int t_id, const int t_max_fuel)
    : m_name_(t_name), m_shape_(t_shape), m_pos_(Grid::NOT_SPAWN)
      , m_speed_(t_speed), m_wait_to_run_(0.0f)
      , m_id_(t_id), m_max_fuel_(t_max_fuel), m_fuel_(t_max_fuel) {}

  //___ public _____________________________________________
  void Car::spawned(const std::shared_ptr<Grid>& t_track)
  {
    m_track_ = t_track;
    m_pos_ = m_track_->spawns(m_id_, m_shape_);
  }

  /*___ Speed ___________________________________
  | slowest speed: 0.1   = 1 tiles/10 secs       |
  | speed:         1.0   = 1 tiles/1000 milisecs |
  | Fastest speed: 500.0 = 1 tiles/2 milisecs    |
  |_____________________________________________*/

  int Car::runs()
  {
    if (m_pos_.X == Grid::NOT_SPAWN.X)
    {
      return 0;
    }

    m_wait_to_run_ += m_speed_ * Game_engine::get_delta_time();

    const auto RUN = 1.0f;
    if(m_wait_to_run_ >= RUN)
    {
      m_wait_to_run_ -= RUN;
      return do_run();
    }
    return m_fuel_;
  }

  void Car::set_id(const int t_id)
  {
    REQUIRE(t_id > 0);

    if(m_pos_.X != Grid::NOT_SPAWN.X)
    {
      m_track_->set(m_pos_, m_id_, t_id);
    }//already spawned on track

    m_id_ = t_id;
  }

  //___ private _____________________________________________
  int Car::do_run()
  {
    switch (--m_fuel_)
    {
    case 0: m_track_->despawns(m_pos_, m_id_); return m_fuel_; default:;
    }

    const auto IS_ON_NORTH = m_pos_.Y < m_track_->get_middle_pos().Y;
    switch(IS_ON_NORTH)
    {
    case true: return go_x_north_or_south(EAST);
    default: return go_x_north_or_south(WEST);
    }
  }

  int Car::go_x_north_or_south(const Direction t_east_or_west)
  {
    const auto IS_ON_WEST = m_pos_.X < m_track_->get_middle_pos().X;
    switch (IS_ON_WEST)
    {
    case true: return random_move_xy_or_else(t_east_or_west, NORTH);
    default: return random_move_xy_or_else(t_east_or_west, SOUTH);
    }
  }

  int Car::random_move_xy_or_else(const int t_x, const int t_y)
  {
    const auto FORWARD_ATTEMPS = 15;
    for (auto i = 1; i <= FORWARD_ATTEMPS; ++i)
    {
      switch (random_move_xy(t_x, t_y))
      {
      case true: return m_fuel_; default:;
      }
    }

    const auto SIDES_ATTEMPS = 4;
    for (auto i = 1; i <= SIDES_ATTEMPS; ++i)
    {
      switch (m_track_->moved(m_pos_, m_id_, m_shape_
        /*_____________________________________________
        | NE => E,   SE => W,   SW => E,   NW => W     |
        | 1+-1 => 1, 1+1 => -1, -1+1 => 1, -1+-1 => -1 |
        | 0 => 1,    2 => -1,   0 => 1,    -2 => -1    |
        |_____________________________________________*/
        , random_2_diagonal_pos(t_x+t_y? WEST: EAST)))
      {
      case true: return m_fuel_; default:;
      }
    }

    random_move_xy(-t_x, -t_y);
    return m_fuel_;
  }

  bool Car::random_move_xy(const int t_x, const int t_y)
  {
    const short MOVE_X = m_pos_.X + static_cast<short>(Game_engine::random(0, t_x))
      , MOVE_Y = m_pos_.Y + static_cast<short>(Game_engine::random(0, t_y));

    if(MOVE_X != m_pos_.X || MOVE_Y != m_pos_.Y)
    {
      return m_track_->moved(m_pos_, m_id_, m_shape_, COORD{ MOVE_X, MOVE_Y });
    }
    return false;
  }

  COORD Car::random_2_diagonal_pos(const Direction t_east_or_west) const
  {
    const short MOVE_EAST_OR_WEST = m_pos_.X + t_east_or_west
      , MOVE_WEST_OR_EAST = m_pos_.X - t_east_or_west
      , MOVE_NORTH = m_pos_.Y + NORTH
      , MOVE_SOUTH = m_pos_.Y + SOUTH;

    return Game_engine::random(false, true) ? 
      COORD{ MOVE_EAST_OR_WEST, MOVE_NORTH }
      : COORD{ MOVE_WEST_OR_EAST, MOVE_SOUTH };
  }

}//jdb