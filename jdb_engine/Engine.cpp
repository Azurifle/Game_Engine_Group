#include <stdafx.h>
#include "Engine.hpp"
#include "System_base.hpp"
#include "Gui_system.hpp"

namespace jdb
{
  //___ public utility ___________________________________________________________

  const float Engine::PRECISION = 0.01f;

  std::string Engine::double_points_string(const double t_double, const int t_points)
  {
    std::stringstream double_w_points;
    double_w_points << std::fixed << std::setprecision(t_points) << t_double;
    return double_w_points.str();
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
    , std::vector<std::vector<Vec3<int>>>& t_image)
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
    t_image.resize(HEIGHT, std::vector<Vec3<int>>(WIDTH, Vec3<int>()));
    const auto DATA_SIZE = (WIDTH * BGR + BGR & ~BGR) * HEIGHT;
    std::vector<char> inverse_row_img(DATA_SIZE);
    bmp_stream.read(inverse_row_img.data(), inverse_row_img.size());

    enum Enum { B, G, R, COLOR_VALUE = 255, ROW_PADDING = 0 };//usually 2 but this time it's 0

    /* 
    //Find row padding size
    for(int i = DATA_SIZE-3; i > 0; i-=3)
    {
      std::cout << i << " : " << (inverse_row_img[i + B] & COLOR_VALUE) << ","
        << (inverse_row_img[i + G] & COLOR_VALUE) << ","
        << (inverse_row_img[i + R] & COLOR_VALUE) << std::endl;
    }*/

    for (unsigned row = 0; row < HEIGHT; ++row)
    {
      for (unsigned col = 0; col < WIDTH; ++col)
      {
        const auto INDEX = (HEIGHT - 1 - row)
          * (WIDTH * BGR + ROW_PADDING) + col * BGR;

        t_image[row][col].x = inverse_row_img[INDEX + B] & COLOR_VALUE;
        t_image[row][col].y = inverse_row_img[INDEX + G] & COLOR_VALUE;
        t_image[row][col].z = inverse_row_img[INDEX + R] & COLOR_VALUE;
      }//col loop
    }//row loop
  }

  nlohmann::json Engine::load_json(const std::string& t_path)
  {
    return nlohmann::json::parse(std::ifstream(t_path));
  }

  //___ public engine ___________________________________________________________

  Engine::State Engine::state() { return m_state_; }

  Engine::Engine()
  {
    REQUIRE(m_state_ <= INVALID);
    m_state_ = CONSTRUCTING;

    disable_mouse_editing();
    srand(GetTickCount());
    show_header();
  }

  Engine::~Engine()
  {
    m_state_ = DESTROYING;
    puts("JDB Engine destroying...");
    m_state_ = INVALID;
    puts("JDB Engine destroyed.");
  }

  void Engine::run(std::unique_ptr<System_base> t_app)
  {
    REQUIRE(t_app);
    m_app_ = std::move(t_app);
    const auto WINDOW = init();
    
    m_app_->start();

    while (!Gui_system::has_window_close(WINDOW))
    {
      m_delta_time_.reset();
      m_app_->update(m_delta_time_.float_seconds());
      Gui_system::swap_buffer_n_clear_inputs(WINDOW);
    }
    
    m_state_ = SHUTTING_DOWN;
    std::cout << "JDB Engine: \"" << m_app_->title() << "\" is shutting down..." << std::endl;
    Gui_system::destroy_windows();
    m_app_.reset();
  }

  //___ private ___________________________________________________________

  Engine::State Engine::m_state_ = INVALID;

  void Engine::disable_mouse_editing()
  {
    DWORD prev_mode;
    PROMISE(GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &prev_mode));
    PROMISE(SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), prev_mode & ~ENABLE_QUICK_EDIT_MODE));
  }

  void Engine::show_header()
  {
    puts("=== JDB Game Engine =======================");
    puts("");
    puts("By: Jee, Dandy, Ball");
    puts("");
    puts("JDB Engine constructed.");
  }

  GLFWwindow* Engine::init()
  {
    m_state_ = INITIALIZING;
    std::cout << "JDB Engine: initailizing \"" << m_app_->title() << "\"." 
      << std::endl << std::endl;

    const auto WINDOW = Gui_system::create_window(m_app_->title(), m_app_->window_size());
    Gui_system::set_key_callback(WINDOW, this, key_callback);

    m_state_ = RUNNING;
    std::cout << "JDB Engine: \"" << m_app_->title() << "\" is running."
      << std::endl << std::endl;

    return WINDOW;
  }

  void Engine::key_callback(GLFWwindow* t_window, const int t_key
    , const int t_scancode, const int t_action, const int t_mods)
  {
    auto that = static_cast<Engine*>(glfwGetWindowUserPointer(t_window));
    REQUIRE(that->m_app_);
    that->m_app_->key_callback(t_window, t_key, t_scancode, t_action, t_mods);
  }

}//jdb