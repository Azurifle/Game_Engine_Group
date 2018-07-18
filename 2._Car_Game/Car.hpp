#ifndef G6037599_CAR_HPP
#define G6037599_CAR_HPP
#pragma once

namespace jdb
{
  class Grid;

  class Car final
  {
  public:
    enum Direction { WEST = -1, EAST = 1, NORTH = -1, SOUTH = 1 };

    Car(const std::string& t_name, const std::string& t_shape
      , float t_speed, int t_id, int t_max_fuel);
    ~Car() = default;
    Car(const Car& t_to_copy) = default;
    Car& operator=(const Car& t_to_copy) = default;
    Car& operator=(Car&& t_to_move) noexcept = default;

    void spawned(const std::shared_ptr<Grid>& t_track);
    int runs();
    void set_id(int t_id);
  private:
    std::string m_name_{}, m_shape_{};
    COORD m_pos_{};
    std::shared_ptr<Grid> m_track_{};
    float m_speed_, m_wait_to_run_;
    int m_id_, m_max_fuel_, m_fuel_;

    Car(Car&& t_to_move) noexcept = default;

    int do_run();
    int go_x_north_or_south(Direction t_east_or_west);
    int random_move_xy_or_else(int t_x, int t_y);
    bool random_move_xy(int t_x, int t_y);
    COORD random_2_diagonal_pos(Direction t_east_or_west) const;
  };
}//jdb

#endif //G6037599_GAME_ENGINE_HPP