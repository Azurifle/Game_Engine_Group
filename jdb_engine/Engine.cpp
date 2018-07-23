#include <stdafx.h>
#include "Engine.hpp"
#include "App.hpp"
#include "shadow_maze/Game.hpp"

namespace jdb
{
  //___ static ___________________________________________________________
  const float Engine::SECOND = 1000.0f;
  const float Engine::PRECISION = 0.01f;

  void Engine::start()
  {
    REQUIRE(!m_is_running_);
    m_is_running_ = true;

    disable_mouse_editing();
    srand(GetTickCount());

    shadow_maze::Game::init();

    puts("============================ End of Program ====================================");
    wait_key();
    m_is_running_ = false;
  }

  bool Engine::is_running() { return m_is_running_; }

  int Engine::get_key()
  {
    switch (_kbhit())
    {
    case KEY_NO_PRESS: return KEY_NO_PRESS;
    default: return wait_key();
    }//switch 1st keyboard hit
  }

  int Engine::wait_key()
  {
    const auto FIRST_KEY = _getch();
    switch (FIRST_KEY)
    {
    case KEY_ARROW: return _getch();
    default: switch (_kbhit())
      {
      case KEY_NO_PRESS: break;
      default: _getch();
      }//switch 2nd keyboard hit
      return FIRST_KEY;
    }
  }

  int Engine::wait_key(const int t_miliseconds)
  {
    const auto TIME_UP = clock() + t_miliseconds;
    do
    {
      const auto KEY = get_key();
      switch (KEY)
      {
      case KEY_NO_PRESS: break;
      default: return KEY;
      }

      Sleep(FPS_50);
    } while (clock() < TIME_UP);

    return KEY_NO_PRESS;
  }

  COORD Engine::get_cursor()
  {
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    PROMISE(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info));
    return console_info.dwCursorPosition;
  }

  void Engine::set_cursor(const COORD& t_coord)
  {
    REQUIRE(0 <= t_coord.X); REQUIRE(t_coord.X <= CMD_LAST_COLS);
    REQUIRE(0 <= t_coord.Y); REQUIRE(t_coord.Y <= CMD_LAST_ROWS);

    PROMISE(SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), t_coord));
  }

  short Engine::limit_interval(const short t_number
    , const short t_min, const short t_max)
  {
    if (t_number <= t_min)
    {
      return t_min;
    }
    if (t_number >= t_max)
    {
      return t_max;
    }
    return t_number;
  }

  void Engine::load_txt(const std::string& t_path, std::string& t_string_out)
  {
    std::ifstream file_reader(t_path);
    REQUIRE(file_reader.is_open());//should change to popup warning later

    std::string temp;
    while (std::getline(file_reader, temp))
    {
      t_string_out += temp;
    }
    file_reader.close();
  }

  void Engine::load_bmp(const std::string& t_path
    , std::vector<std::vector<std::vector<int>>>& t_image)
  {
    //adapt from https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file

    std::ifstream bmp_stream(t_path, std::ios::binary);
    REQUIRE(bmp_stream.is_open());//should change to popup warning later

    const size_t HEADER_SIZE = 54;
    std::array<char, HEADER_SIZE> header;
    bmp_stream.read(header.data(), header.size());

    const unsigned WIDTH = *reinterpret_cast<unsigned *>(&header[18])
      , HEIGHT = *reinterpret_cast<unsigned *>(&header[22])
      , BGR = 3;
    t_image.resize(HEIGHT, std::vector<std::vector<int>>(WIDTH, std::vector<int>(BGR)));

    const auto DATA_SIZE = (WIDTH * BGR + BGR & ~BGR) * HEIGHT;
    std::vector<char> inverse_row_img(DATA_SIZE);
    bmp_stream.read(inverse_row_img.data(), inverse_row_img.size());

    enum Enum { B, G, R, COLOR_VALUE = 255, ROW_PADDING = 2 };
    for (unsigned row = 0; row < HEIGHT; ++row)
    {
      for (unsigned col = 0; col < WIDTH; ++col)
      {
        const auto INDEX = (HEIGHT - 1 - row)
          * (WIDTH * BGR + ROW_PADDING) + col * BGR;

        t_image[row][col][B] = inverse_row_img[INDEX + B] & COLOR_VALUE;
        t_image[row][col][G] = inverse_row_img[INDEX + G] & COLOR_VALUE;
        t_image[row][col][R] = inverse_row_img[INDEX + R] & COLOR_VALUE;
      }//col loop
    }//row loop
  }

  int Engine::random(int t_min, int t_max)
  {
    if(t_min > t_max)
    {
      std::swap(t_min, t_max);
    }
    return t_min + (rand() & t_max - t_min);
  }

  void Engine::reset_delta_time()
  {
    m_delta_milisec_ = clock();
  }

  float Engine::get_delta_time()
  {
    return (clock() - m_delta_milisec_)/SECOND;
  }

  std::string Engine::double_points_string(const double t_double, const int t_points)
  {
    std::stringstream double_w_points;
    double_w_points << std::fixed << std::setprecision(t_points) << t_double;
    return double_w_points.str();
  }

  void Engine::paint_pos(const Vec3<float>& t_pos, const Vec3<float>& t_rgb)
  {
    glColor3f(t_rgb.x, t_rgb.y, t_rgb.z);
    glVertex3f(t_pos.x, t_pos.y, t_pos.z);
  }

  //___ private static ___________________________________________________________
  clock_t Engine::m_delta_milisec_ = 0;
  bool Engine::m_is_running_ = false;

  void Engine::disable_mouse_editing()
  {
    DWORD prev_mode;
    PROMISE(GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &prev_mode));
    PROMISE(SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), prev_mode & ~ENABLE_QUICK_EDIT_MODE));
  }

  void Engine::show_header()
  {
    puts("=== Game Engine =======================");
    puts("");
    puts("JDB_Engine");
    puts("");
    puts("");
    puts("         ////////////////////////// Game list /////////////////////////////");
    puts("");
    puts("                    1. The Fantasy World - NoOne The Hero.");
    puts("");
    puts("                    2. Bit Autonomous Car.");
    puts("");
    puts("                    3. Shadow Maze.");
    puts("");
    puts("         //////////////////////////////////////////////////////////////////");
    puts("");
    puts("");
  }

  void Engine::back_to_main_menu()
  {
    std::cout << std::endl
      << "Press <Any key> to main menu: ";
    _getch(); _getch();
    system("CLS");
  }
}//jdb