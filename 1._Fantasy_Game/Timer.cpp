#include "stdafx.h"
#include "Timer.hpp"
#include "Console.hpp"

namespace G6037599
{
  //___ (de)constructors _____________________________________________
  Timer::Timer(const COORD& t_pos) : m_start_(t_pos), m_day_(1)
    , m_hour_(0), m_minute_(0) {}

  //___ public _______________________________________________________
  void Timer::show()
  {
    Console::set_cursor(m_start_);
    std::cout << std::setw(WIDTH) << std::setfill('=') << '=';

    m_day_ = 1;
    m_hour_ = 0;
    m_minute_ = 0;

    Console::set_cursor({m_start_.X, m_start_.Y+ ROW_TIME });
    std::cout << "| " << std::setw(DAY_FIELD_WIDTH) << std::setfill('0') 
      << m_day_ << " Day " << std::setw(HOUR_FIELD_WIDTH) << std::setfill('0')
      << m_hour_ << ':' << m_minute_ << " Hour |";

    Console::set_cursor({ m_start_.X, m_start_.Y + ROW_BOTTOM });
    std::cout << std::setw(WIDTH) << std::setfill('=') << '=';
  }

  int Timer::update_minute()
  {
    ++m_minute_;
    const auto ONE_HOUR = 6;
    if(m_minute_ >= ONE_HOUR)
    {
      return update_hour();
    }
    Console::set_cursor({ m_start_.X + MINUTE_FIELD_X, m_start_.Y + ROW_TIME });
    std::cout << m_minute_;
    return NOT_REACH;
  }

  int Timer::update_hour()
  {
    m_minute_ = 0;
    ++m_hour_;
    const auto ONE_DAY = 24;
    if (m_hour_ >= ONE_DAY)
    {
      m_hour_ = 0;
      const auto DAY_LIMIT = 10000;
      m_day_ = (m_day_ + 1) % DAY_LIMIT;

      Console::set_cursor({ m_start_.X + DAY_FIELD_X, m_start_.Y + ROW_TIME });
      std::cout << std::setw(DAY_FIELD_WIDTH) << std::setfill('0') << m_day_;

      Console::set_cursor({ m_start_.X + HOUR_FIELD_X, m_start_.Y + ROW_TIME });
      std::cout << std::setw(HOUR_FIELD_WIDTH) << std::setfill('0') << m_hour_;

      Console::set_cursor({ m_start_.X + MINUTE_FIELD_X, m_start_.Y + ROW_TIME });
      std::cout << '0';
      return DAY_REACH;
    }
    Console::set_cursor({ m_start_.X + HOUR_FIELD_X, m_start_.Y + ROW_TIME });
    std::cout << std::setw(HOUR_FIELD_WIDTH) << std::setfill('0') << m_hour_;

    Console::set_cursor({ m_start_.X + MINUTE_FIELD_X, m_start_.Y + ROW_TIME });
    std::cout << '0';
    return HOUR_REACH;
  }
}//G6037599