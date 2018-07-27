#include <stdafx.h>
#include "Player.hpp"

namespace shadow_maze
{
  Player::Player(): m_pos_(jdb::Vec3<float>(NOT_SPAWN)) {}

  void Player::spawn_on(const std::shared_ptr<Map>& t_map)
  {
    m_map_ = t_map;
    //m_map_->spawn_pos(m_pos_);
  }

  void Player::move_up()
  {
    REQUIRE(m_pos_.x >= NOT_SPAWN);
    //m_map_->move_up();
  }

  void Player::move_down()
  {
    REQUIRE(m_pos_.x >= NOT_SPAWN);
  }

  void Player::move_left()
  {
    REQUIRE(m_pos_.x >= NOT_SPAWN);
  }

  void Player::move_right()
  {
    REQUIRE(m_pos_.x >= NOT_SPAWN);
  }
}//shadow_maze