// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "0._Game_Engine/Game_engine.hpp"

int main()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  //jdb::Game_engine::start();
  auto pEngine = std::make_shared<jdb::Game_engine>();
  int result = pEngine->start();
  return result;
}
