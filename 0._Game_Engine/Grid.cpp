#include "stdafx.h"
#include "Grid.hpp"
#include "Game_engine.hpp"

namespace G6037599
{
  //___ static _____________________________________________
  const COORD Grid::NOT_SPAWN = { -1, -1 };

  //___ (de)constructors _____________________________________________
  Grid::Grid(const COORD& t_start_coord, const COORD& t_end_coord)
    : m_start_coord_(t_start_coord), m_end_coord_(t_end_coord)
  {
    REQUIRE(0 <= t_start_coord.X); REQUIRE(t_end_coord.X <= Game_engine::CMD_LAST_COLS);
    REQUIRE(0 <= t_start_coord.Y); REQUIRE(t_end_coord.Y <= Game_engine::CMD_LAST_ROWS);
    REQUIRE(t_start_coord.X < t_end_coord.X);
    REQUIRE(t_start_coord.Y < t_end_coord.Y);
  }

  //___ public _____________________________________________
  void Grid::load(const std::string& t_bmp_path)
  {
    m_tiles_.clear(); 
    clean_canvas();

    std::vector<std::vector<std::vector<int>>> image;
    Game_engine::load_bmp(t_bmp_path, image);

    m_tiles_.resize(image.size()
      , std::vector<Tile>(image[0].size(), Tile{ NO_TILE, NO_TILE })
    );

    enum Color{ 
      B, G, R
      , CAN_MOVE_B = 176, CAN_MOVE_G = 228, CAN_MOVE_R = 239 
    };

    for (unsigned row = 0; row < m_tiles_.size(); ++row)
    {
      for (unsigned col = 0; col < m_tiles_[0].size(); ++col)
      {
        if (image[row][col][B] == CAN_MOVE_B
          && image[row][col][G] == CAN_MOVE_G
          && image[row][col][R] == CAN_MOVE_R)
        {
          m_tiles_[row][col].slot_1 = NO_GAME_OBJECT;
          m_tiles_[row][col].slot_2 = NO_GAME_OBJECT;
        }
      }//col loop
    }//row loop

    align_center(m_print_coord_.X, m_print_size_.X
      , m_start_coord_.X, m_end_coord_.X, static_cast<short>(m_tiles_[0].size()), COLS_PER_TILE);

    align_center(m_print_coord_.Y, m_print_size_.Y
      , m_start_coord_.Y, m_end_coord_.Y, static_cast<short>(m_tiles_.size()) );
  }

  void Grid::print() const
  {
    short row = 0;
    do
    {
      Game_engine::set_cursor({ m_print_coord_.X, m_print_coord_.Y + row });

      for (short col = 0; col < m_print_size_.X; ++col)
      {
        switch (m_tiles_[row][col].slot_1)
        {
        case NO_TILE: std::cout << "##+"; break;
        default: std::cout << "   ";
        }
      }
      ++row;
    } while (row < m_print_size_.Y);
  }

  COORD Grid::spawns(const int t_id, const std::string& t_symbol)
  {
    REQUIRE(t_id > NO_GAME_OBJECT);
    COORD pos;
    unsigned i = 0;
    const auto LOOP_LIMIT = m_tiles_.size();
    do
    {//change to find nearest available tile if have time
      pos.Y = static_cast<short>(rand()) % m_print_size_.Y;
      pos.X = static_cast<short>(rand()) % m_print_size_.X;
      ++i;
      if(i > LOOP_LIMIT) { return NOT_SPAWN; }
    } while (m_tiles_[pos.Y][pos.X].slot_1 != NO_GAME_OBJECT
      || m_tiles_[pos.Y][pos.X].slot_2 != NO_GAME_OBJECT);

    m_tiles_[pos.Y][pos.X].slot_1 = t_id;
    Game_engine::set_cursor(get_coord(pos));
    std::cout << t_symbol;

    return pos;
  }

  void Grid::despawns(const COORD& t_pos, const int t_id)
  {
    REQUIRE(t_pos.X != NOT_SPAWN.X);
    REQUIRE(t_id > NO_GAME_OBJECT);

    if (m_tiles_[t_pos.Y][t_pos.X].slot_1 == t_id)
    {
      m_tiles_[t_pos.Y][t_pos.X].slot_1 = NO_GAME_OBJECT;

      Game_engine::set_cursor(get_coord(t_pos));
      std::cout << '.';
    }
    else
    {
      REQUIRE(m_tiles_[t_pos.Y][t_pos.X].slot_2 == t_id);
      m_tiles_[t_pos.Y][t_pos.X].slot_2 = NO_GAME_OBJECT;

      Game_engine::set_cursor(get_coord(t_pos, true));
      std::cout << ' ';
    }
  }

  bool Grid::moved(COORD& t_pos, const int t_id
    , const std::string& t_symbol, COORD t_moved)
  {
    REQUIRE(t_pos.X != NOT_SPAWN.X);
    REQUIRE(t_id > NO_GAME_OBJECT);

    t_moved.X = Game_engine::limit_interval(t_moved.X, 0, m_print_size_.X);
    t_moved.Y = Game_engine::limit_interval(t_moved.Y, 0, m_print_size_.Y);

    switch (m_tiles_[t_moved.Y][t_moved.X].slot_1)
    {
    case NO_GAME_OBJECT: move_to_slot_1(t_pos, t_id, t_symbol, t_moved); break;
    case NO_TILE: return false;
    default: switch (m_tiles_[t_moved.Y][t_moved.X].slot_2)
      {
      case NO_GAME_OBJECT: move_to_slot_2(t_pos, t_id, t_symbol, t_moved); break;
      default: return false;
      }
    }
    t_pos = t_moved;
    return true;    
  }

  void Grid::set(const COORD& t_pos, const int t_id, const int t_new_id)
  {
    REQUIRE(t_pos.X != NOT_SPAWN.X && t_pos.Y != NOT_SPAWN.Y);
    REQUIRE(t_id > NO_GAME_OBJECT); REQUIRE(t_new_id > NO_GAME_OBJECT);
    REQUIRE(t_id != t_new_id);

    if (m_tiles_[t_pos.Y][t_pos.X].slot_1 == t_id)
    {
      m_tiles_[t_pos.Y][t_pos.X].slot_1 = t_new_id;
    }
    else
    {
      REQUIRE(m_tiles_[t_pos.Y][t_pos.X].slot_2 == t_id);
      m_tiles_[t_pos.Y][t_pos.X].slot_2 = t_new_id;
    }
  }

  COORD Grid::get_middle_pos()
  {
    return COORD{static_cast<short>(m_tiles_[0].size()/2)
      , static_cast<short>(m_tiles_.size()/2)};
  }

  //___ private static ______________________________________
  void Grid::align_center(short& t_print_coord_out, short& t_print_size_out
    , const short t_start_coord, const short t_end_coord
    , const short t_size, const short t_tile_size)
  {
    const short LAST_PRINT_COORD = t_start_coord + t_size * t_tile_size - 1;
    if (LAST_PRINT_COORD >= t_end_coord)
    {
      t_print_coord_out = t_start_coord;
      t_print_size_out = (t_end_coord - t_start_coord) / t_tile_size;
    }
    else
    {
      const short SPACE_BEFORE = (t_end_coord - t_start_coord + 1
        - t_size * t_tile_size) / 2;
      t_print_coord_out = t_start_coord + SPACE_BEFORE;
      t_print_size_out = t_size;
    }
  }

  //___ private _____________________________________________
  COORD Grid::get_coord(const COORD& t_pos, const bool t_is_slot_2) const
  {
    REQUIRE(0 <= t_pos.X); REQUIRE(t_pos.X < m_print_size_.X);
    REQUIRE(0 <= t_pos.Y); REQUIRE(t_pos.Y < m_print_size_.Y);

    return COORD{ m_print_coord_.X + t_pos.X * COLS_PER_TILE + (t_is_slot_2 ? 1: 0)
      , m_print_coord_.Y + t_pos.Y };
  }

  void Grid::clean_canvas() const
  {
    short row = 0;
    do
    {
      Game_engine::set_cursor({ m_start_coord_.X, m_start_coord_.Y + row });
      std::cout << std::setw(m_end_coord_.X - m_start_coord_.X + 1) << std::setfill(' ') << ' ';
      ++row;
    } while (row < m_end_coord_.Y);
  }

  void Grid::move_symbol_n_despawn(const COORD& t_pos, const int t_id
    , const std::string& t_symbol, const COORD& t_moved, const bool t_is_slot2)
  {
    Game_engine::set_cursor(get_coord(t_moved, t_is_slot2));
    std::cout << t_symbol;
    despawns(t_pos, t_id);
  }

  void Grid::move_to_slot_1(const COORD& t_pos, const int t_id
    , const std::string& t_symbol, const COORD& t_moved)
  {
    m_tiles_[t_moved.Y][t_moved.X].slot_1 = t_id;
    move_symbol_n_despawn(t_pos, t_id, t_symbol, t_moved);
  }

  void Grid::move_to_slot_2(const COORD& t_pos, const int t_id
    , const std::string& t_symbol, const COORD& t_moved)
  {
    m_tiles_[t_moved.Y][t_moved.X].slot_2 = t_id;
    move_symbol_n_despawn(t_pos, t_id, t_symbol, t_moved, true);
  }

}//G6037599