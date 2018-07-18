#ifndef JDB_DELTA_TIME_HPP
#define JDB_DELTA_TIME_HPP
#pragma once

namespace jdb
{
  class Delta_time final
  {
  public:
    Delta_time();
    ~Delta_time() = default;
    Delta_time(const Delta_time& t_to_copy) = default;
    Delta_time(Delta_time&& t_to_move) noexcept = default;
    Delta_time& operator = (const Delta_time& t_to_copy) = default;
    Delta_time& operator = (Delta_time&& t_to_move) noexcept = default;

    void reset();
    float float_seconds() const;
  private:
    using Clock = std::chrono::high_resolution_clock;

    std::chrono::time_point<std::chrono::steady_clock> m_time_now_{};
    std::chrono::duration<float> m_delta_seconds_{};
  };
}//jdb

#endif //JDB_DELTA_TIME_HPP
