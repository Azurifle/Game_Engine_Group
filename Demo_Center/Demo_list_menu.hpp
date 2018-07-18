#ifndef G6037599_DEMO_LIST_MENU_HPP
#define G6037599_DEMO_LIST_MENU_HPP
#pragma once

namespace G6037599
{
  class Demo_list_menu final
  {
  public:
    static void start();
  private:
    static void show_menu();
    static void run_app();
    static void press_to_exit();

    Demo_list_menu() = default;
    ~Demo_list_menu() = default;
    Demo_list_menu(const Demo_list_menu& t_to_copy) = default;
    Demo_list_menu(Demo_list_menu&& t_to_move) noexcept = default;
    Demo_list_menu& operator = (const Demo_list_menu& t_to_copy) = default;
    Demo_list_menu& operator = (Demo_list_menu&& t_to_move) noexcept = default;
  };
}//G6037599

#endif //G6037599_DEMO_LIST_MENU_HPP