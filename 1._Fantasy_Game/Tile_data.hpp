#ifndef G6037599_TILE_DATA_HPP
#define G6037599_TILE_DATA_HPP
#pragma once

namespace G6037599
{
  class Tile_data final
  {
  public:
    const int ID;

    Tile_data();
    explicit Tile_data(const COORD& t_pos);
    ~Tile_data() = default;

    COORD get_pos() const;
    void set_pos(const COORD& t_pos);
  private:
    COORD m_pos_{};
    static int m_id_count_;

    Tile_data(const Tile_data& t_to_copy) = default;
    Tile_data(Tile_data&& t_to_move) noexcept = default;
    Tile_data& operator=(const Tile_data& t_to_copy) = default;
    Tile_data& operator=(Tile_data&& t_to_move) noexcept = default;
  };
}//G6037599

#endif //G6037599_TILE_DATA_HPP