#ifndef G6037599_TIMER_HPP
#define G6037599_TIMER_HPP
#pragma once

namespace G6037599
{
  class Timer final
  {
  public:
    const enum Enum { NOT_REACH, HOUR_REACH, DAY_REACH, ROW_TIME = 1, ROW_BOTTOM
      , HOUR_FIELD_WIDTH = 2, DAY_FIELD_WIDTH = 4, WIDTH = 22 
      , DAY_FIELD_X = 2, HOUR_FIELD_X = 11, MINUTE_FIELD_X = 14};

    explicit Timer(const COORD& t_pos);
    ~Timer() = default;
    Timer(const Timer& t_to_copy) = default;//no (smart)pointer
    Timer(Timer&& t_to_move) noexcept = default;
    Timer& operator=(const Timer& t_to_copy) = default;
    Timer& operator=(Timer&& t_to_move) noexcept = default;

    void show();
    int update_minute();
    int update_hour();
  private:
    COORD m_start_{};
    int m_day_, m_hour_, m_minute_;
  };
}//G6037599

#endif //G6037599_TIMER_HPP