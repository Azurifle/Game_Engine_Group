#ifndef G6037599_SPAWN_POINT_HPP
#define G6037599_SPAWN_POINT_HPP
#pragma once
#include "Tile_data.hpp"

namespace G6037599
{
  class Type_data;
  class Map;
  class Console;

  class Spawn_point
  {
  public:
    Spawn_point(std::shared_ptr<Type_data> t_type
      , std::shared_ptr<Console> t_m_console, std::shared_ptr<Map> t_m_map);
    virtual ~Spawn_point() = default;
    Spawn_point(const Spawn_point& t_to_copy);
    Spawn_point(Spawn_point&& t_to_move) noexcept = default;
    Spawn_point& operator=(const Spawn_point& t_to_copy);
    Spawn_point& operator=(Spawn_point&& t_to_move) noexcept = default;

    COORD get_pos() const;
    int get_id() const;
    std::string get_type_name() const;
    int get_type_max_hp() const;
    int get_type_atk() const;
    int get_type_max_atk() const;
    int get_type_behavior() const;
    std::string get_symbol() const;
    std::shared_ptr<Type_data> share_type() const;

    void set_pos(const COORD& t_pos) const;
    int random_type_atk() const;
    void reset_type() const;

    virtual int get_last_id() const = 0;
    virtual int find_hp(int t_id) const = 0;
    virtual bool damaged_n_dies(int t_monster_id, int t_damage) const = 0;
    virtual void spawn(int t_monsters) = 0;
    virtual void monsters_stronger(int t_percent) const = 0;
    virtual COORD find_pos(int t_monster_id) const = 0;
  protected:
    void set_console_monster_hp(int t_hp, int t_max_hp) const;
    COORD random_unoccupied_tile() const;
    void map_move(const COORD& t_from, int t_id, const COORD& t_to) const;
    void mark_map(const COORD& t_pos, int t_id) const;
    void mark_console(const COORD& t_pos, bool t_is_remove = false) const;
    void increase_type_max_hp(int t_amount) const;
  private:
    std::shared_ptr<Type_data> m_type_{};
    std::shared_ptr<Console> m_console_{};
    std::shared_ptr<Map> m_map_{};
    std::unique_ptr<Tile_data> m_tile_{};
  };
}//G6037599

#endif //G6037599_SPAWN_POINT_HPP