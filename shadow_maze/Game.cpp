#include <stdafx.h>
#include "Game.hpp"
#include "jdb_engine/Engine.hpp"
#include "jdb_engine/Audio_manager.hpp"
#include "jdb_engine/App.hpp"

namespace shadow_maze
{
  //___ public __________________________________________________________________________________

  void Game::init()
  {
    static auto is_running = false;
    REQUIRE(jdb::Engine::is_running());
    REQUIRE(!is_running);
    is_running = true;

    puts(" === Shadow Maze =======================");
    puts("");
    puts("By: JDB Engine Group");
    puts("");

    const auto BGM = jdb::Audio_manager::load_or_get_audio("shadow_maze/music/Game.wav");
    jdb::Audio_manager::play(BGM);

    jdb::App app(jdb::Vec2<int>(900, 600), "Shadow Maze");
    app.run();

    jdb::Audio_manager::stop(BGM);

    is_running = false;
  }

  Game::~Game()
  {
  }

  //___ private __________________________________________________________________________________

  const std::string Game::GAME_FOLDER("shadow_maze/");

  Game::Game()
  {
    
  }
}//shadow_maze