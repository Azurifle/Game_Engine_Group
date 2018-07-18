#include "stdafx.h"
#include "Car_game.hpp"
#include "0._Game_Engine/Game_engine.hpp"
#include "0._Game_Engine/Grid.hpp"
#include "../Thirdparty/json.hpp"

namespace jdb
{
  //___ static _____________________________________________
  const std::string Car_game::GAME_FOLDER("2._Car_Game/");

  void Car_game::init(bool start)
  {
	  const COORD TRACK_START = { 1, 3 }, TRACK_END = { 90, 35 };
	  m_track_ = std::make_shared<Grid>(TRACK_START, TRACK_END);

	  const auto JSON_CONFIG = nlohmann::json::parse(std::ifstream(
		  GAME_FOLDER + "0. Config.json"));

	  m_track_names_ = std::move(JSON_CONFIG["track_names"].get<std::vector<std::string>>());
	  m_track_->load(GAME_FOLDER + m_track_names_[m_current_track_] + ".bmp");
	  m_track_->print();

	  std::vector<std::string> car_types = JSON_CONFIG["car_names"];
	  for (unsigned i = 0; i < car_types.size(); ++i)
	  {
		  const auto JSON_CAR = nlohmann::json::parse(
			  std::ifstream(GAME_FOLDER + car_types[i] + ".json"));

		  m_cars_.push_back(std::make_unique<Car>(
			  car_types[i], JSON_CAR["shape"].get<std::string>()
			  , JSON_CAR["speed"].get<float>()
			  , i, JSON_CAR["max_fuel"].get<int>()
			  ));
	  }
	  m_1_st_spawned_ = m_cars_.size();
  }

  void Car_game::run()
  {
    //REQUIRE(Game_engine::is_running());
    //REQUIRE(!m_is_running_);
    //m_is_running_ = true;

    //Car_game game;

    while (true)
    {
      Game_engine::reset_delta_time();
      Sleep(Game_engine::FPS_50);
      switch (Game_engine::get_key())
      {
      case Game_engine::KEY_SPACE: spawns_car(); break;
      case 'w': changes_track(-1); break;
      case 's': changes_track(1); break;
      case Game_engine::KEY_ESC: Game_engine::shutdown_engine(); break;//m_is_running_ = false; 
      //  return; default:;
      }
      update();
    }//game loop
  }

  //___ private static _____________________________________________
  //bool Car_game::m_is_running_ = false;

  //___ private constructor _____________________________________________
  Car_game::Car_game() : m_1_st_spawned_(Grid::NO_GAME_OBJECT+1)
    , m_current_track_(0)
  {
   
  }

  //___ private _____________________________________________
  void Car_game::update()
  {
    for(auto i = m_1_st_spawned_; i < m_cars_.size(); ++i)
    {
      switch(m_cars_[i]->runs())
      {
      case 0: m_cars_.erase(m_cars_.begin() + i); 
        for (auto replace = i; replace < m_cars_.size(); ++replace)
        {
          m_cars_[replace]->set_id(replace);
        } break; default:;
      }//fuel empty
    }//each cars runs
  }

  void Car_game::spawns_car()
  {
    m_cars_.push_back(std::make_unique<Car>(*m_cars_[rand() % m_1_st_spawned_]));
    m_cars_[m_cars_.size() -1]->set_id(m_cars_.size() - 1);
    m_cars_[m_cars_.size() - 1]->spawned(m_track_);
  }

  void Car_game::changes_track(int t_move)
  {
    t_move = Game_engine::limit_interval(
      static_cast<short>(m_current_track_ + t_move)
      , 0, static_cast<short>(m_track_names_.size()) - 1
    );

    if(t_move != static_cast<int>(m_current_track_))
    {
      m_cars_.erase(m_cars_.begin() + m_1_st_spawned_, m_cars_.end());
      m_track_->load(GAME_FOLDER + m_track_names_[t_move] + ".bmp");
      m_track_->print();
      m_current_track_ = t_move;
    }
  }
}//jdb