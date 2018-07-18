#include "stdafx.h"
#include "Console.hpp"
#include "Fantasy_game.hpp"
#include "Map.hpp"
#include "Timer.hpp"

namespace jdb
{
  //___ static _______________________________________________________
  COORD Console::get_cursor()
  {
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info))
    {
      return console_info.dwCursorPosition;
    }
    const COORD ORIGIN = { 0, 0 };
    return ORIGIN;
  }

  void Console::set_cursor(const COORD& t_pos)
  {
    REQUIRE(0 <= t_pos.X);
    REQUIRE(0 <= t_pos.Y);

    PROMISE(SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), t_pos));
  }

  void Console::print_centered_text(const int t_width, const char* t_message, const char t_delim)
  {
    const auto SPACE_BEFORE = (t_width - std::strlen(t_message)) / 2
      , SPACE_AFTER = SPACE_BEFORE + SPACE_BEFORE % 2;
    std::cout << std::setw(SPACE_BEFORE) << std::setfill(t_delim) << t_delim;
    std::cout << t_message << std::setw(SPACE_AFTER) << std::setfill(t_delim) << t_delim;
  }

  //___ (de)constructors _____________________________________________
  Console::Console()
  {
    REQUIRE(std::strlen(Fantasy_game::PLAYER_NAME) <= Status_panel::MAX_NAME_LENGTH);

    auto cursor = get_cursor();
    const short TO_MONSTER_STATUS = 7;
    cursor.X += TO_MONSTER_STATUS;
    ++cursor.Y;
    m_monster_status_ = std::make_unique<Status_panel>(cursor);

    ++cursor.Y;
    m_map_start_ = { 2, cursor.Y + Status_panel::HIEGHT};

    const short TO_TIMER_PANEL = 7;
    cursor.X += Status_panel::WIDTH + TO_TIMER_PANEL;
    ++cursor.Y;
    m_timer_ = std::make_unique<Timer>(cursor);

    const short TO_PLAYER_HP_BAR = 9;
    cursor.X += Timer::WIDTH + TO_PLAYER_HP_BAR;
    m_player_hp_ = std::make_unique<Hp_bar>(cursor);

    const short TO_CURSOR_STATUS_COLUMN = 5, TO_CURSOR_STATUS_ROW = 11;
    cursor.X = Map::SIZE * SPACE_BETWEEN_TILE + TO_CURSOR_STATUS_COLUMN;
    cursor.Y += TO_CURSOR_STATUS_ROW;
    m_cursor_status_ = std::make_unique<Status_panel>(cursor);

    m_pop_up_panel_cursor_ = find_map_cursor_pos({ Map::MIDDLE / 3, Map::MIDDLE / 2 });
    cursor = m_pop_up_panel_cursor_;

    const short TO_BEHIND_MESSAGE = (POP_UP_PANEL_WIDTH + 
      static_cast<short>( std::strlen(GAME_RESET_MESSAGE) ) ) / 2
      , BACK_TO_COUNT_DOWN_COLUMN = 2, GAME_RESET_ROW = 4;
    m_game_reset_count_down_cursor_ = { cursor.X + TO_BEHIND_MESSAGE
      - BACK_TO_COUNT_DOWN_COLUMN, cursor.Y + GAME_RESET_ROW };
  }

  //___ public _______________________________________________________
  void Console::show() const
  {
    system("CLS");
    m_timer_->show();
    
    print_player_status();

    auto cursor = m_cursor_status_->get_pos();
    const short UP_TO_OBJECTIVE_TITLE = 10, ROW_DOWN = 2;
    cursor.Y -= UP_TO_OBJECTIVE_TITLE;
    set_cursor(cursor);

    std::cout << "=== Quest ===";
    ++cursor.X;
    cursor.Y += ROW_DOWN;
    set_cursor(cursor);

    std::cout << "1. Slain all monsters before";
    ++cursor.Y;
    set_cursor(cursor);
    std::cout << "  they become too strong !!.";
    cursor.Y += ROW_DOWN;
    set_cursor(cursor);

    std::cout << "2. NoOne The Hero must survive.";
    
    print_map();

    cursor = m_cursor_status_->get_pos();
    const short UP_TO_CURSOR_TITLE = 2;
    cursor.Y -= UP_TO_CURSOR_TITLE;
    set_cursor(cursor);
    std::cout << "=== Cursor ===";

    const short TO_CURSOR_INSTRUCT = 2;
    cursor.Y += static_cast<short>(m_cursor_status_->HIEGHT) + TO_CURSOR_INSTRUCT;
    set_cursor(cursor);
    std::cout << "    [/\\]";
    ++cursor.Y;
    set_cursor(cursor); 
    std::cout << " [<][\\/][>] = move cursor";

    const short TO_SYMBOL_INSTRUCT = 5;
    cursor.Y += TO_SYMBOL_INSTRUCT;
    set_cursor(cursor);
    std::cout << "=== Hint ===";
    cursor.Y += ROW_DOWN;
    set_cursor(cursor);
    std::cout << "&     = You, the player, the hero.";
    cursor.Y += ROW_DOWN;
    set_cursor(cursor);
    std::cout << "X     = Spawner, spawn monster.";
    cursor.Y += ROW_DOWN;
    set_cursor(cursor);
    std::cout << "Other = Monsters, stronger each day.";
    cursor.Y += ROW_DOWN;
    set_cursor(cursor);
    std::cout << "- Kill Zombie Doramon to get";
    ++cursor.Y;
    set_cursor(cursor);
    std::cout << "  instant healing device!";

    const short TO_WSAD_INSTRUCT_COLUMN = 15, TO_WSAD_INSTRUCT_ROW = 1;
    cursor = { m_map_start_.X + TO_WSAD_INSTRUCT_COLUMN
      , m_map_start_.Y + Map::SIZE + TO_WSAD_INSTRUCT_ROW };
    set_cursor(cursor);
    std::cout << "   [W]";
    ++cursor.Y;
    set_cursor(cursor);
    std::cout << "[A][S][D] = move";

    const short TO_SPACE_INSTRUCT = 40;
    cursor.X += TO_SPACE_INSTRUCT;
    set_cursor(cursor);
    std::cout << "[SPACE] = force update";

    const short TO_ESC_INSTRUCT = 5;
    cursor.X = Map::SIZE * SPACE_BETWEEN_TILE + TO_ESC_INSTRUCT;
    --cursor.Y;
    set_cursor(cursor);
    std::cout << "[ESC] = exit";
  }

  void Console::print_player_status() const
  {
    auto cursor = m_player_hp_->get_pos();
    enum Enum { BACK_TO_EDGE = 2, PLAYER_STATUS_WIDTH = 24 };
    cursor.X -= BACK_TO_EDGE;
    --cursor.Y;
    set_cursor(cursor);
    std::cout << "== \" " << Fantasy_game::PLAYER_NAME << " \" ==";

    ++cursor.Y;
    set_cursor(cursor);
    std::cout << '|';
    m_player_hp_->set(Fantasy_game::PLAYER_MAX_HP, Fantasy_game::PLAYER_MAX_HP);
    std::cout << std::setw(cursor.X + PLAYER_STATUS_WIDTH - get_cursor().X) << std::setfill(' ') << '|';

    ++cursor.Y;
    set_cursor(cursor);
    std::cout << "| ATK: " << Fantasy_game::PLAYER_ATK << " - " << Fantasy_game::PLAYER_MAX_ATK;
    std::cout << std::setw(cursor.X + PLAYER_STATUS_WIDTH - get_cursor().X) << std::setfill(' ') << '|';

    ++cursor.Y;
    set_cursor(cursor);    
    std::cout << std::setw(PLAYER_STATUS_WIDTH) << std::setfill('=') << '=';
  }

  void Console::print_map() const
  {
    for (short row = 0; row < Map::SIZE; ++row)
    {
      set_cursor({ m_map_start_.X, m_map_start_.Y + row });
      for (short column = 0; column < Map::SIZE; ++column)
      {
        std::cout << ".  ";
      }
    }
  }

  void Console::marked(const COORD& t_pos, const char* t_symbol, const bool t_is_attacker) const
  {
    REQUIRE(0 <= t_pos.X && t_pos.X < Map::SIZE);
    REQUIRE(0 <= t_pos.Y && t_pos.Y < Map::SIZE);

    const auto SLOT_SIZE = 2;
    switch (t_is_attacker)
    {
    case false: set_cursor(find_map_cursor_pos(t_pos));
      REQUIRE(std::strlen(t_symbol) <= SLOT_SIZE); break;
    default: const auto SLOT_ATTACKER = 1;
      REQUIRE(std::strlen(t_symbol) <= SLOT_ATTACKER);
      set_cursor(COORD{ find_map_cursor_pos(t_pos).X+ SLOT_ATTACKER
        , find_map_cursor_pos(t_pos).Y });
    }
    std::cout << t_symbol;

    set_cursor(m_player_cursor_);
  }

  void Console::move_player_cursor(const COORD& t_pos)
  {
    REQUIRE(0 <= t_pos.X && t_pos.X < Map::SIZE);
    REQUIRE(0 <= t_pos.Y && t_pos.Y < Map::SIZE);

    m_player_cursor_ = find_map_cursor_pos(t_pos);
    set_cursor(m_player_cursor_);
  }

  int Console::update_minute() const
  {
    const int result = m_timer_->update_minute();
    set_cursor(m_player_cursor_);
    return result;
  }

  int Console::update_hour() const
  {
    const int result = m_timer_->update_hour();
    set_cursor(m_player_cursor_);
    return result;
  }

  void Console::hide_cursor_status() const
  {
    m_cursor_status_->hide();//keep bool alreay hide or not
    set_cursor(m_player_cursor_);
  }

  void Console::show_cursor_status(const std::string& t_name, const int t_hp
    , const int t_max_hp, const int t_atk, const int t_max_atk) const
  {
    m_cursor_status_->show(t_name, t_hp, t_max_hp, t_atk, t_max_atk);
    set_cursor(m_player_cursor_);
  }

  void Console::hide_monster_status() const
  {
    m_monster_status_->hide();//keep bool alreay hide or not
    set_cursor(m_player_cursor_);
  }

  void Console::show_monster_status(const std::string& t_name, const int t_hp
    , const int t_max_hp, const int t_atk, const int t_max_atk) const
  {
    m_monster_status_->show(t_name, t_hp, t_max_hp, t_atk, t_max_atk);
    set_cursor(m_player_cursor_);
  }

  void Console::thanks_user()
  {
    puts("");
    puts("");
    puts("");
    puts("                      /////////////////////// THE FANTASY WORLD ///////////////////");
    puts("");
    puts("                                       By: Darlyn Sirikasem jdb.");
    puts("");
    puts("");
    puts("                                     THANK YOU FOR PLAYING THIS GAME !!!");
    puts("");
    puts("");
    puts("                      ////////////////////////////////////////////////////////////");
    puts("");
  }

  void Console::show_game_reset() const
  {
    auto cursor = m_pop_up_panel_cursor_;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill('/') << '/';
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill(' ') << ' ';
    ++cursor.Y;
    set_cursor(cursor);

    print_centered_text(POP_UP_PANEL_WIDTH,
      (std::string("\" ") + Fantasy_game::PLAYER_NAME + std::string(" \" has fallen.")).c_str()
      , ' ');
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill(' ') << ' ';
    ++cursor.Y;
    set_cursor(cursor);

    print_centered_text(POP_UP_PANEL_WIDTH, GAME_RESET_MESSAGE, ' ');
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill(' ') << ' ';
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill('/') << '/';
    set_cursor(m_game_reset_count_down_cursor_);
  }

  void Console::show_count_down(const int t_count_down) const
  {
    std::cout << t_count_down;
    set_cursor(m_game_reset_count_down_cursor_);
  }

  void Console::show_next_stage() const
  {
    auto cursor = m_pop_up_panel_cursor_;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill('/') << '/';
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill(' ') << ' ';
    ++cursor.Y;
    set_cursor(cursor);

    print_centered_text(POP_UP_PANEL_WIDTH,
      (std::string("WELL DONE \" ") + Fantasy_game::PLAYER_NAME + std::string(" \" !!!.")).c_str()
      , ' ');
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill(' ') << ' ';
    ++cursor.Y;
    set_cursor(cursor);

    print_centered_text(POP_UP_PANEL_WIDTH, "Next stage in ?.", ' ');
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill(' ') << ' ';
    ++cursor.Y;
    set_cursor(cursor);

    std::cout << std::setw(POP_UP_PANEL_WIDTH) << std::setfill('/') << '/';
    set_cursor(m_game_reset_count_down_cursor_);
  }

  void Console::set_player_full_hp() const
  {
    m_player_hp_->set(Fantasy_game::PLAYER_MAX_HP, Fantasy_game::PLAYER_MAX_HP);
  }

  void Console::set_player_hp(const int t_hp) const
  {
    REQUIRE(0 <= t_hp && t_hp <= Fantasy_game::PLAYER_MAX_HP);

    m_player_hp_->set(t_hp, Fantasy_game::PLAYER_MAX_HP);
    set_cursor(m_player_cursor_);
  }

  void Console::set_monster_hp(const int t_hp, const int t_max_hp) const
  {
    m_monster_status_->set(t_hp, t_max_hp);
  }

  void Console::set_cursor_hp(const int t_hp, const int t_max_hp) const
  {
    m_cursor_status_->set(t_hp, t_max_hp);
  }

  //___ private _______________________________________________________
  COORD Console::find_map_cursor_pos(const COORD& t_pos) const
  {
    REQUIRE(0 <= t_pos.X && t_pos.X < Map::SIZE);
    REQUIRE(0 <= t_pos.Y && t_pos.Y < Map::SIZE);

    return COORD{ m_map_start_.X + t_pos.X * SPACE_BETWEEN_TILE, m_map_start_.Y + t_pos.Y };
  }

}//jdb
