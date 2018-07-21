#include "stdafx.h"
#include "Shadow_maze.hpp"
#include "0._Game_Engine/Game_engine.hpp"
#include "0._Game_Engine/Visual_Manager/Renderer.hpp"
#include "0._Game_Engine/Audio_manager.hpp"
#include "0._Game_Engine/App.hpp"

namespace jdb
{
  //___ public __________________________________________________________________________________

  void Shadow_maze::run()
  {
    static auto is_running = false;
    REQUIRE(Game_engine::is_running());
    REQUIRE(!is_running);
    is_running = true;

    puts(" === Shader App with sound =======================");
    puts("");
    puts("By: JDB Engine Group");
    puts("");

    const auto BGM = Audio_manager::load_or_get_audio("1._Shadow_Maze/music/Game.wav");
    Audio_manager::play(BGM);

    App app(640, 480, "Rendering a Triangle!!");
    app.run();

    Audio_manager::stop(BGM);

    is_running = false;
  }

  Shadow_maze::~Shadow_maze()
  {
  }

  //___ private __________________________________________________________________________________

  const std::string Shadow_maze::GAME_FOLDER("3._Shadow_Maze/");

  Shadow_maze::Shadow_maze()
  {
    
  }

  void Shadow_maze::update()
  {

  }
}//jdb