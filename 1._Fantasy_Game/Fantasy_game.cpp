#include "stdafx.h"
#include "Fantasy_game.hpp"
#include "Console.hpp"
#include "Map.hpp"
#include "Type_data.hpp"
#include "Timer.hpp"
#include "Spawn_to.hpp"

namespace jdb
{
  //___ static _____________________________________________
  const char* Fantasy_game::MONSTER_CONF_PATH = "1._Fantasy_Game/monster_conf.txt";
  const char* Fantasy_game::PLAYER_NAME = "NoOne The Hero";
  const COORD Fantasy_game::UP = { 0, -1 };
  const COORD Fantasy_game::DOWN = { 0, 1 };
  const COORD Fantasy_game::LEFT = { -1, 0 };
  const COORD Fantasy_game::RIGHT = { 1, 0 };

  void Fantasy_game::runs()
  {
    REQUIRE(Game_engine::is_running());
    REQUIRE(!m_is_running_);
    m_is_running_ = true;
    Fantasy_game world;

    while (true)
    {
      const auto HALF_SECOND = 500;
      switch (Game_engine::wait_key(HALF_SECOND))
      {
      case Game_engine::KEY_NO_PRESS: world.update(); break;
      case 'w': world.player_move(UP); break;
      case 's': world.player_move(DOWN); break;
      case 'a': world.player_move(LEFT); break;
      case 'd': world.player_move(RIGHT); break;
      case Game_engine::KEY_ARROW_UP: world.move_cursor(UP); break;
      case Game_engine::KEY_ARROW_DOWN: world.move_cursor(DOWN); break;
      case Game_engine::KEY_ARROW_LEFT: world.move_cursor(LEFT); break;
      case Game_engine::KEY_ARROW_RIGHT: world.move_cursor(RIGHT); break;
      case Game_engine::KEY_ESC: world.exit(); 
        m_is_running_ = false; return;
      default: world.player_move();
      }
    }//game loop
  }

  short Fantasy_game::limit_interval(int t_number, const int t_min, const int t_max)
  {
    if (t_number < t_min)
      t_number = t_min;
    else if (t_number > t_max)
      t_number = t_max;
    return static_cast<short>(t_number);
  }

  //___ (de)constructors _____________________________________________
  Fantasy_game::Fantasy_game() : m_map_(std::make_shared<Map>())
    , m_console_(std::make_shared<Console>())
    , m_player_(std::make_unique<Unit>(std::make_shared<Type_data>(
      PLAYER_NAME, "Ahhhhh, sh*t I'm dead.", "Punch!"
    , PLAYER_MAX_HP, PLAYER_ATK, PLAYER_MAX_ATK, '&')))
    , m_monster_count_(STAGE_1_MONSTERS), m_level_monsters_(STAGE_1_MONSTERS)
  {
    m_console_->show();

    m_player_->set_pos(COORD{ Map::MIDDLE, Map::MIDDLE });
    m_player_cursor_pos_ = m_player_->get_pos();
    m_map_->marked(m_player_->get_pos(), m_player_->get_id());

    m_console_->move_player_cursor(m_player_->get_pos());
    m_console_->marked(m_player_->get_pos(), std::string(1, m_player_->get_symbol()).c_str());

    read_monster_types();
    spawn_spawners();
    spawners_spawn_monster();
  }

  //___ public _____________________________________________
  void Fantasy_game::player_move()
  {
    switch (m_console_->update_hour())
    {
    case Timer::HOUR_REACH: check_battle(); 
      break;
    default: monsters_stronger();
      update_monster_status_hp();
      update_cursor_status_hp();
    }//switch update_hour
  }

  void Fantasy_game::player_move(COORD t_move)
  {
    t_move.X = limit_interval(m_player_->get_pos().X + t_move.X, 0, m_map_->SIZE -1);
    t_move.Y = limit_interval(m_player_->get_pos().Y + t_move.Y, 0, m_map_->SIZE -1);

    for (auto it = m_spawners_.begin(); it != m_spawners_.end(); ++it)
    {
      if (t_move.X == (*it)->get_pos().X && t_move.Y == (*it)->get_pos().Y)
      {
        update();
        return;
      }
    }//way blocked by spawner

    switch (m_console_->update_hour())
    {
    case Timer::HOUR_REACH: break;
    default: monsters_stronger();
      update_cursor_status_hp();
    }//switch update_hour

    switch (m_map_->is_attacker(m_player_->get_pos(), m_player_->get_id()))
    {
    case false: m_console_->marked(m_player_->get_pos(), "."); 
      break;
    default: m_console_->marked(m_player_->get_pos(), " ", true);
    }//move from battle?

    m_map_->move(m_player_->get_pos(), m_player_->get_id(), t_move);
    m_player_->set_pos(t_move);

    switch (m_map_->is_attacker(m_player_->get_pos(), m_player_->get_id()))
    {
    case false: m_console_->marked(t_move, std::string(1, m_player_->get_symbol()).c_str());
      m_console_->hide_monster_status();
      break;
    default: player_move_into_battle();
    }//move into battle?    
  }

  void Fantasy_game::move_cursor(const COORD& t_move)
  {
    m_player_cursor_pos_.X = limit_interval(m_player_cursor_pos_.X + t_move.X, 0, m_map_->SIZE);
    m_player_cursor_pos_.Y = limit_interval(m_player_cursor_pos_.Y + t_move.Y, 0, m_map_->SIZE);
    m_console_->move_player_cursor(m_player_cursor_pos_);
    
    const auto ENEMY_ID = m_map_->get(m_player_cursor_pos_);
    if(ENEMY_ID <= m_map_->NO_UNIT || ENEMY_ID == m_player_->get_id())
    {
      m_console_->hide_cursor_status();
    }
    else
    {
      for(auto it = m_spawners_.begin(); it < m_spawners_.end(); ++it)
      {
        if(ENEMY_ID == (*it)->get_id())
        {
          m_console_->hide_cursor_status();
          return;
        }
      }//check that it's not spawner

      const auto INDEX = find_spawner_index(ENEMY_ID);      
      m_console_->show_cursor_status(m_spawners_[INDEX]->get_type_name()
        , m_spawners_[INDEX]->find_hp(ENEMY_ID)
        , m_spawners_[INDEX]->get_type_max_hp()
        , m_spawners_[INDEX]->get_type_atk()
        , m_spawners_[INDEX]->get_type_max_atk());
    }//has unit that not player
  }

  void Fantasy_game::update()
  {
    switch (m_console_->update_minute())
    {
    case Timer::NOT_REACH: break;
    case Timer::HOUR_REACH: check_battle(); break;
    default: monsters_stronger();
      update_monster_status_hp();
      update_cursor_status_hp();
    }//switch update minute
  }

  void Fantasy_game::exit() const
  {
    system("CLS");
    m_console_->thanks_user();
  }

  //___ private static ________________________________________________
  bool Fantasy_game::m_is_running_ = false;

  //___ private _______________________________________________________
  void Fantasy_game::read_monster_types()
  {
    std::vector<std::string> tokens;
    Game_engine::load_txt(MONSTER_CONF_PATH, tokens);

    enum Enum 
    { 
      NAME, DEAD_MESSAGE, ATK_NAME, MAX_HP, ATK, MAX_ATK, BEHAVIOR, SYMBOL
      , TOKENS_PER_LINE
    };

    for(unsigned line = 0; line < tokens.size(); line += TOKENS_PER_LINE)
    {
      m_spawners_.push_back(std::make_unique< Spawn_to<Unit> >( 
        std::make_shared<Type_data>(tokens[NAME+ line], tokens[DEAD_MESSAGE + line]
          , tokens[ATK_NAME + line], std::stoi(tokens[MAX_HP + line])
          , std::stoi(tokens[ATK + line]), std::stoi(tokens[MAX_ATK + line])
          , tokens[SYMBOL + line][0], std::stoi(tokens[BEHAVIOR + line]) )
        , m_console_, m_map_) );
    }
  }

  void Fantasy_game::spawn_spawners()
  {
    for (unsigned i = 0; i < m_spawners_.size(); ++i)
    {
      m_spawners_[i]->reset_type();
      m_spawners_[i]->set_pos(m_map_->random_unoccupied());
      m_map_->marked(m_spawners_[i]->get_pos(), m_spawners_[i]->get_id());
      m_console_->marked(m_spawners_[i]->get_pos(), m_spawners_[i]->get_symbol().c_str());
    }
  }

  void Fantasy_game::spawners_spawn_monster()
  {
    REQUIRE(m_monster_count_ > 0);
    REQUIRE(m_monster_count_ <= m_map_->SIZE * m_map_->SIZE);

    std::vector<int> size_per_types;
    const auto FIRST_MONSTER = 1;
    for (unsigned i = 0; i < m_spawners_.size(); ++i)
    {
      size_per_types.push_back(FIRST_MONSTER);
    }

    const auto AFTER_FIRST_MONSTER = m_spawners_.size();
    for (auto i = AFTER_FIRST_MONSTER; i < static_cast<unsigned>(m_monster_count_); ++i)
    {
      ++size_per_types[rand() % m_spawners_.size()];
    }

    for (unsigned i = 0; i < m_spawners_.size(); ++i)
    {
      m_spawners_[i]->spawn(size_per_types[i]);
    }
  }

  void Fantasy_game::monsters_stronger()
  {
    for (auto it = m_spawners_.begin(); it != m_spawners_.end(); ++it)
    {
      const auto PERCENT = 1;
      (*it)->monsters_stronger(PERCENT);
    }
  }

  void Fantasy_game::game_reset()
  {
    const auto THREE_SECOND = 3000, COUNT_DOWN = 9, ONE_SECOND = 1000;
    Game_engine::wait_key(THREE_SECOND);

    m_console_->show_game_reset();
    for(auto i = COUNT_DOWN; i > 0; --i)
    {
      m_console_->show_count_down(i);
      switch (Game_engine::wait_key(ONE_SECOND))
      {
      case Game_engine::KEY_NO_PRESS: break;
      default: _getch();
      }
    }

    m_console_->show();

    m_map_->reset();

    m_player_->set_pos({ Map::MIDDLE , Map::MIDDLE });
    m_player_->set_hp(PLAYER_MAX_HP);
    m_map_->marked(m_player_->get_pos(), m_player_->get_id());
    m_console_->marked(m_player_->get_pos()
      , std::string(1, m_player_->get_symbol()).c_str());
    m_console_->set_player_full_hp();

    spawn_spawners();
    m_monster_count_ = m_level_monsters_;
    spawners_spawn_monster();
    m_player_cursor_pos_ = m_player_->get_pos();
    m_console_->move_player_cursor(m_player_cursor_pos_);
  }

  void Fantasy_game::check_battle()
  {
    const auto ENEMY_ID = m_map_->get(m_player_->get_pos());
    if(ENEMY_ID == m_player_->get_id())
    {
      return;
    }

    const auto INDEX = find_spawner_index(ENEMY_ID);
    switch ( m_spawners_[INDEX]->damaged_n_dies(ENEMY_ID, m_player_->random_atk() ) )
    {
    case false: update_cursor_status_hp();
      
      m_player_->damaged(m_spawners_[INDEX]->random_type_atk());
      m_console_->set_player_hp(m_player_->get_hp());

      switch (m_player_->get_hp())
      {
        case 0: game_reset(); default:;
      } 
      break;
    default: monster_dies(INDEX);
    }//switch monster dies
    
  }

  int Fantasy_game::find_spawner_index(const int t_monster_id) const
  {
    REQUIRE(t_monster_id > Map::NO_UNIT);

    for (unsigned i = 0; i < m_spawners_.size(); ++i)
    {
      if (t_monster_id <= m_spawners_[i]->get_last_id())
      {
        return i;
      }
    }
    PROMISE(false);//it's not monster id
    const auto NO_SPAWNER = -1;
    return NO_SPAWNER;
  }

  void Fantasy_game::update_monster_status_hp() const
  {
    const auto ENEMY_ID = m_map_->get(m_player_->get_pos());
    if (ENEMY_ID != m_player_->get_id())
    {
      const auto INDEX = find_spawner_index(ENEMY_ID);
      m_console_->set_monster_hp(m_spawners_[INDEX]->find_hp(ENEMY_ID)
        , m_spawners_[INDEX]->get_type_max_hp());
    }//player is attacker
  }

  void Fantasy_game::update_cursor_status_hp() const
  {
    const auto ENEMY_ID = m_map_->get(m_player_cursor_pos_);
    if (ENEMY_ID > m_map_->NO_UNIT && ENEMY_ID != m_player_->get_id())
    {
      for (auto it = m_spawners_.begin(); it < m_spawners_.end(); ++it)
      {
        if (ENEMY_ID == (*it)->get_id())
        {
          return;
        }
      }//check that it's not spawner

      const auto INDEX = find_spawner_index(ENEMY_ID);
      m_console_->set_cursor_hp(m_spawners_[INDEX]->find_hp(ENEMY_ID)
        , m_spawners_[INDEX]->get_type_max_hp());
    }
  }

  void Fantasy_game::player_move_into_battle()
  {
    m_console_->marked(m_player_->get_pos()
      , std::string(1, m_player_->get_symbol()).c_str(), true);

    const auto ENEMY_ID = m_map_->get(m_player_->get_pos())
      , INDEX = find_spawner_index(ENEMY_ID);
    m_console_->show_monster_status(m_spawners_[INDEX]->get_type_name()
      , m_spawners_[INDEX]->find_hp(ENEMY_ID)
      , m_spawners_[INDEX]->get_type_max_hp()
      , m_spawners_[INDEX]->get_type_atk()
      , m_spawners_[INDEX]->get_type_max_atk());
  }

  void Fantasy_game::monster_dies(const int t_index)
  {
    const auto INSTANT_HEAL = 1;
    switch(m_spawners_[t_index]->get_type_behavior())
    {
    case INSTANT_HEAL: m_player_->set_hp(PLAYER_MAX_HP);
      m_console_->set_player_full_hp(); default:;
    }

    m_console_->hide_monster_status();

    m_map_->move(m_player_->get_pos(), m_player_->get_id(), m_player_->get_pos());
    m_console_->marked(m_player_->get_pos(), " ", true);
    m_console_->marked(m_player_->get_pos(), std::string(1, m_player_->get_symbol()).c_str());
    
    if (m_player_cursor_pos_.X == m_player_->get_pos().X
      && m_player_cursor_pos_.Y == m_player_->get_pos().Y)
    {
      m_console_->hide_cursor_status();
    }

    --m_monster_count_;
    switch (m_monster_count_) { case 0: next_stage(); default:; }
  }

  void Fantasy_game::next_stage()
  {
    const auto THREE_SECOND = 3000, COUNT_DOWN = 9, ONE_SECOND = 1000;
    Game_engine::wait_key(THREE_SECOND);

    m_console_->show_next_stage();
    for (auto i = COUNT_DOWN; i > 0; --i)
    {
      m_console_->show_count_down(i);
      switch (Game_engine::wait_key(ONE_SECOND))
      {
      case Game_engine::KEY_NO_PRESS: break;
      default: _getch();
      }
    }

    m_console_->show();

    m_map_->reset();

    m_player_->set_pos({ Map::MIDDLE , Map::MIDDLE });
    m_player_->set_hp(PLAYER_MAX_HP);
    m_map_->marked(m_player_->get_pos(), m_player_->get_id());
    m_console_->marked(m_player_->get_pos()
      , std::string(1, m_player_->get_symbol()).c_str());
    m_console_->set_player_full_hp();

    spawn_spawners();

    const auto MONSTER_MULTIPLY = 2;
    m_level_monsters_ *= MONSTER_MULTIPLY;
    m_monster_count_ = m_level_monsters_;    
    spawners_spawn_monster();

    m_player_cursor_pos_ = m_player_->get_pos();
    m_console_->move_player_cursor(m_player_cursor_pos_);
  }
}//namespace jdb