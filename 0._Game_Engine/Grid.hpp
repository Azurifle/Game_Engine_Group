#ifndef G6037599_GRID_HPP
#define G6037599_GRID_HPP
#pragma once

namespace jdb
{
  class Grid final
  {
  public:
    enum Enum { NO_TILE = -2, NO_GAME_OBJECT, COLS_PER_TILE = 3};
    static const COORD NOT_SPAWN;

    Grid(const COORD& t_start_coord, const COORD& t_end_coord);
    ~Grid() = default;

    void load(const std::string& t_bmp_path);
    void print() const;
    COORD spawns(int t_id, const std::string& t_symbol);
    void despawns(const COORD& t_pos, int t_id);
    bool moved(COORD& t_pos, int t_id
      , const std::string& t_symbol, COORD t_moved);
    void set(const COORD& t_pos, int t_id, int t_new_id);
    COORD get_middle_pos();
  private:
    static void align_center(short& t_print_coord_out
      , short& t_print_size_out
      , short t_start_coord, short t_end_coord,
      short t_size, short t_tile_size = 1);

    struct Tile final { int slot_1, slot_2; };

    std::vector<std::vector<Tile>> m_tiles_;
    COORD m_start_coord_{}, m_end_coord_{}
      , m_print_coord_{}, m_print_size_{};

    Grid(const Grid& t_to_copy) = default;
    Grid(Grid&& t_to_move) noexcept = default;
    Grid& operator=(const Grid& t_to_copy) = default;
    Grid& operator=(Grid&& t_to_move) noexcept = default;

    COORD get_coord(const COORD& t_pos, bool t_is_slot_2 = false) const;
    void clean_canvas() const;
    void move_symbol_n_despawn(const COORD& t_pos, int t_id
      , const std::string& t_symbol
      , const COORD& t_moved, bool t_is_slot2 = false);
    void move_to_slot_1(const COORD& t_pos, int t_id
      , const std::string& t_symbol, const COORD& t_moved);
    void move_to_slot_2(const COORD& t_pos, int t_id
      , const std::string& t_symbol, const COORD& t_moved);
  };
}//jdb

#endif //G6037599_GRID_HPP