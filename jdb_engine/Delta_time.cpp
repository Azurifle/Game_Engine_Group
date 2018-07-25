#include <stdafx.h>
#include "Delta_time.hpp"

namespace jdb
{
  // ___ constructor __________________________________________________________
  Delta_time::Delta_time(): m_time_now_(Clock::now()){}

  // ___ public _______________________________________________________________
  void Delta_time::reset()
  {
    const auto TIME_THEN = m_time_now_;
    m_time_now_ = Clock::now();
    m_delta_seconds_ = m_time_now_ - TIME_THEN;
  }

  float Delta_time::float_seconds() const
  {
    return m_delta_seconds_.count();
  }
}//jdb