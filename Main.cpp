// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "jdb_engine/Engine.hpp"

int main()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  jdb::Engine::start();
}
