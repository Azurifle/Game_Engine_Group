#include <stdafx.h>
#include "Demo_list_menu.hpp"
#include "Demo_Center.hpp"
#include "Week_12/App.hpp"
#include "Week_13/Audio_manager.hpp"

namespace G6037599
{
  // ___ static ___________________________________________________________
  void Demo_list_menu::start()
  {
    static auto is_running = false;
    REQUIRE(!is_running);
    is_running = true;

    Demo_center::disable_mouse_editing();
    show_menu();
    run_app();
    press_to_exit();
    
    is_running = false;
  }

  // ___ private ___________________________________________________________
  void Demo_list_menu::show_menu()
  {
    puts(" === Shader App with sound =======================");
    puts("");
    puts("By: Darlyn Sirikasem G6037599");
    puts("");
  }

  void Demo_list_menu::run_app()
  {
    const auto BGM = Audio_manager::load_or_get_audio("Week_13/Winning.wav");
    Audio_manager::play(BGM);

    App app(640, 480, "Rendering a Triangle!!");
    app.run();

    Audio_manager::stop(BGM);
  }

  void Demo_list_menu::press_to_exit()
  {
    puts("");
    Demo_center::print_centered_header("End of Demo", '=');
    puts("");
    std::cout << "Press <Any key> to exit: ";
    Demo_center::wait_key();
  }
}//G6037599

