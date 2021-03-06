// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "jdb_engine/Engine.hpp"
#include "shadow_maze/Game.hpp"

int main()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  jdb::Engine engine;
  std::unique_ptr<jdb::System_base> shadow_maze(new shadow_maze::Game());
  engine.run(std::move(shadow_maze));
}
