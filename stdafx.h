// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#define CRTDBG_MAP_ALLOC

#include <vector>
#include <chrono>//time in nanosec
#include <memory>//smart_ptr
#include <conio.h>//_getch()
#include <iostream>//cout
#include <string>
#include <iomanip>//std::setprecision(2)
#include <sstream>//std::stringstream
#include <map>
#include <string>
#include <fstream>//file stream
#include <memory>//smart ptr
#include <ctime>//clock()
#include <array>//bmp reading

#include <windows.h>
#include <consoleapi.h>//ENABLE_QUICK_EDIT_MODE
#include <winbase.h>//GetStdHandle
#include <winuser.h>
#include <processenv.h>
//#include <consoleapi2.h>
#include <winbase.h>

#include <cassert>
#define REQUIRE(condition) assert(condition)//validate input
#define PROMISE(condition) assert(condition)//validate output

#include "jdb_engine/thirdparty/glew-2.1.0/include/GL/glew.h"
#include "jdb_engine/thirdparty/glfw-3.2.1.bin.WIN32/include/GLFW/glfw3.h"
#include "jdb_engine/thirdparty/alut/include/AL/alut.h"