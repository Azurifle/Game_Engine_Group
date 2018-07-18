#ifndef G6037599_TYPE_DATA_HPP
#define G6037599_TYPE_DATA_HPP
#pragma once

namespace jdb
{
  class Type_data final
  {
  public:
    const std::string NAME{}, DEAD_MESSAGE{}, ATK_NAME{};
    const int ATK = 0, MAX_ATK = 0, ORIGIN_MAX_HP = 0, BEHAVIOR = 0;
    const char SYMBOL = 'U';

    Type_data(const std::string& t_name
      , const std::string& t_dead_message
      , const std::string& t_atk_name
      , int t_max_hp, int t_atk, int t_max_atk
      , char t_symbol, int t_behavior = 0);
    ~Type_data() = default;

    int get_max_hp() const;
    void increase_max_hp(int t_amount);
    void reset_max_hp();
    int random_atk() const;
  private:
    int m_max_hp_;

    Type_data(const Type_data& t_to_copy) = default;
    Type_data(Type_data&& t_to_move) noexcept = default;
    Type_data& operator=(const Type_data& t_to_copy) = default;
    Type_data& operator=(Type_data&& t_to_move) noexcept = default;
  };
}//jdb

#endif //G6037599_TYPE_DATA_HPP
