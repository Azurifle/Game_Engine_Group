#include <stdafx.h>
#include "Gui_system.hpp"
#include "visual/Renderer.hpp"
#include "Engine.hpp"

namespace jdb
{
  //adapt from https://github.com/redeian/FirstGLFW/tree/master/FirstGLFW

  // ___ public ________________________________________________

  GLFWwindow* Gui_system::create_window(const std::string& t_title, const Vec2<int>& t_size)
  {
    switch (glfwInit())
    {
    case GLFW_FALSE: glfwTerminate(); PROMISE(false); default:;
    }

    const auto WINDOW = glfwCreateWindow(t_size.x, t_size.y, t_title.c_str(), nullptr, nullptr);
    if (!WINDOW)
    {
      glfwTerminate(); PROMISE(false);
    }

    glfwMakeContextCurrent(WINDOW);

    init_glew();

    return WINDOW;
  }

  void Gui_system::set_key_callback(GLFWwindow* const t_window, Engine* t_engine,
    void(* t_key_callback)(struct GLFWwindow*, int, int, int, int))
  {
    glfwSetWindowUserPointer(t_window, t_engine);
    glfwSetKeyCallback(t_window, t_key_callback);
  }

  bool Gui_system::has_window_close(GLFWwindow* const t_window)
  {
    return glfwWindowShouldClose(t_window);
  }

  void Gui_system::swap_buffer_n_clear_inputs(GLFWwindow* const t_window)
  {
    glfwSwapBuffers(t_window);
    glfwPollEvents();
  }

  void Gui_system::destroy_windows() { glfwTerminate(); }

  // ___ private _________________________________________________________________________________

  void Gui_system::init_glew()
  {
    const auto RESULT = glewInit();
    switch (RESULT)
    {
    case GLEW_OK: break;
    default: std::cout << "GLEW error: " << glewGetErrorString(RESULT) << std::endl;
      PROMISE(false);
    }

    show_glew_versions();
    test_depth_n_alpha_bend();
  }

  void Gui_system::show_glew_versions()
  {
    std::cout << "Graphic Driver OpenGL version: " << glGetString(GL_VERSION) << std::endl
      << "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) 
      << std::endl << std::endl;
  }

  void Gui_system::test_depth_n_alpha_bend()
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

}//jdb