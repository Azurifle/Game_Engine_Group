#ifndef G6037599_CAR_GAME_HPP
#define G6037599_CAR_GAME_HPP
#pragma once
#include "Car.hpp"
#include "0._Game_Engine/Game_engine.hpp"

namespace jdb
{
  class Grid;

  class Car_game : public System_base
  {
  public:
    static const std::string GAME_FOLDER;

	void init(bool start = true) override;
	void run() override;

	Car_game();
    ~Car_game() = default;
  private:
    //static bool m_is_running_;

    std::vector<std::string> m_track_names_{};
    std::vector<std::unique_ptr<Car>> m_cars_;
    std::shared_ptr<Grid> m_track_{};
    unsigned m_1_st_spawned_, m_current_track_;

    
    Car_game(const Car_game& t_to_copy) = default;
    Car_game(Car_game&& t_to_move) noexcept = default;
    Car_game& operator=(const Car_game& t_to_copy) = default;
    Car_game& operator=(Car_game&& t_to_move) noexcept = default;

    void update();
    void spawns_car();
    void changes_track(int t_move);
  };
}//jdb

#endif //G6037599_CAR_GAME_HPP