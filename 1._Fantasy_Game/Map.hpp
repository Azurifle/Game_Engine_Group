#ifndef G6037599_MAP_HPP
#define G6037599_MAP_HPP
#pragma once

namespace G6037599
{
  class Map final
  {
  public:
    const enum Enum 
    { 
      NO_UNIT = -1, SIZE = 30, MIDDLE = SIZE / 2
    };
    struct Tile { int owner_id = NO_UNIT, attacker_id = NO_UNIT; };

    Map();
    ~Map() = default;
    Map(const Map& t_to_copy);
    Map(Map&& t_to_move) noexcept = default;
    Map& operator=(const Map& t_to_copy);
    Map& operator=(Map&& t_to_move) noexcept = default;

    void marked(const COORD& t_pos, int t_id);
    bool is_attacker(const COORD& t_pos, int t_id) const;
    void move(const COORD& t_from, int t_id, const COORD& t_to);
    int get(const COORD& t_pos) const;
    COORD random_unoccupied() const;
    void reset();
  private:
    std::vector<std::unique_ptr< std::vector<Tile> >> m_grid_;

    void copy_from(const Map& t_to_copy);
  };
}//G6037599

#endif //G6037599_MAP_HPP