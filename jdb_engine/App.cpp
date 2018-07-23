#include <stdafx.h>
#include "App.hpp"
#include "visual/Vertex.hpp"
#include "visual/Shader_manager.hpp"
#include "visual/Renderer.hpp"
#include "visual/Mesh_manager.hpp"
#include "visual/Texture_manager.hpp"

namespace jdb
{
  //adapt from https://github.com/redeian/FirstGLFW/tree/master/FirstGLFW

  // ___ (de)constructors ________________________________________________

  App::App(const Vec2<int>& t_size, const std::string& t_title) 
    : m_size_(t_size), m_is_active_(true)
  {
    init_glfw_system(t_title);
    
    const auto RESULT = glewInit();
    switch (RESULT) 
    { 
      case GLEW_OK: break;
      default: std::cout << "GLEW error: " << glewGetErrorString(RESULT) << std::endl; 
        PROMISE(false);
    }

    show_glew_versions();
    test_depth_n_alpha_bend();

    m_textures_.push_back(Texture_manager::load_or_get(TEXTURE_FOLDER + "Test.png"));
    m_textures_.push_back(Texture_manager::load_or_get(TEXTURE_FOLDER + "Grass.png", GL_RGB));
    m_textures_.push_back(Texture_manager::load_or_get(TEXTURE_FOLDER + "Wall_top.png", GL_RGB));
    m_textures_.push_back(Texture_manager::load_or_get(TEXTURE_FOLDER + "Wall_side.png", GL_RGB));
    m_textures_.push_back(Texture_manager::load_or_get(TEXTURE_FOLDER + "Smiley.png"));

    const auto TEX_LEFT_TOP = Vec2<float>(0, 0), TEX_RIGHT_TOP = Vec2<float>(1, 0)
      , TEX_RIGHT_BOTTOM = Vec2<float>(1, 1), TEX_LEFT_BOTTOM = Vec2<float>(0, 1);

    Mesh_manager::new_mesh(GL_TRIANGLES, Shader_manager::load_or_get());
      Mesh_manager::add_vertex(Vec3<float>(6, -4), TEX_RIGHT_BOTTOM);
      Mesh_manager::add_vertex(Vec3<float>(-6, -4), TEX_LEFT_BOTTOM);
      Mesh_manager::add_vertex(Vec3<float>(0, 6), Vec2<float>(0.5f, 0));
    m_test_mesh_ = Mesh_manager::save_mesh();

    Mesh_manager::new_mesh(GL_QUADS, Shader_manager::load_or_get());
      static const auto TILE_SIZE = 5.0f;
      Mesh_manager::add_vertex(Vec3<float>(-TILE_SIZE, TILE_SIZE), TEX_LEFT_TOP);
      Mesh_manager::add_vertex(Vec3<float>(TILE_SIZE, TILE_SIZE), TEX_RIGHT_TOP);
      Mesh_manager::add_vertex(Vec3<float>(TILE_SIZE, -TILE_SIZE), TEX_RIGHT_BOTTOM);
      Mesh_manager::add_vertex(Vec3<float>(-TILE_SIZE, -TILE_SIZE), TEX_LEFT_BOTTOM);
    m_tile_mesh_ = Mesh_manager::save_mesh();
  }

  App::~App()
  {
    glfwTerminate();//close window
    puts("Glfw_window destroyed.");
  }

  // ___ public ________________________________________________

  void App::run() const
  {
    while (!glfwWindowShouldClose(m_window_))
    {
      const Vec3<float> BLACK(0, 0, 0);
      Renderer::render_bg(BLACK);
      switch (m_is_active_) 
      { 
        case true: glViewport(0, 0, static_cast<int>(round(m_size_.x * 0.25))
          , static_cast<int>(round(m_size_.y * 0.25))); //test mini map
          render_objects();
          glViewport(0, 0, m_size_.x, m_size_.y); render_objects();
        default:; 
      }
      glfwSwapBuffers(m_window_);
      glfwPollEvents();//clear inputs
    }
  }

  // ___ private ________________________________________________

  const std::string App::TEXTURE_FOLDER = "shadow_maze/texture/";

  void App::key_callback_static(GLFWwindow* t_window, const int t_key
    , const int t_scancode, const int t_action, const int t_mods)
  {
    auto that = static_cast<App*>(glfwGetWindowUserPointer(t_window));
    that->key_callback(t_window, t_key, t_scancode, t_action, t_mods);
  }

  void App::show_glew_versions()
  {
    std::cout 
      << "Graphic Driver OpenGL version: " << glGetString(GL_VERSION) << std::endl 
      << "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) 
      << std::endl;
  }

  void App::test_depth_n_alpha_bend()
  {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void App::init_glfw_system(const std::string& t_title)
  {
    switch (glfwInit())
    {
    case GLFW_FALSE: glfwTerminate(); PROMISE(false); default:;
    }

    m_window_ = glfwCreateWindow(m_size_.x, m_size_.y, t_title.c_str(), nullptr, nullptr);
    if (!m_window_)
    {
      glfwTerminate(); PROMISE(false);
    }

    glfwMakeContextCurrent(m_window_);
    glfwSetWindowUserPointer(m_window_, this);
    glfwSetKeyCallback(m_window_, key_callback_static);
  }

  void App::key_callback(GLFWwindow* t_window, const int t_key
    , const int t_scancode, const int t_action, const int t_mods)
  {
    if (t_key == GLFW_KEY_E && t_action == GLFW_PRESS)
    {
      m_is_active_ = !m_is_active_;
      std::cout << (m_is_active_? "Activate window: " :"Deactivate window: ") << t_window
        << ", scancode: " << t_scancode << ", mods: " << t_mods << std::endl;
    }
  }

  void App::render_objects() const
  {
    enum Texture_id { TEST, GRASS, WALL_TOP, WALL_SIDE, SMILEY };

    Renderer::push_matrix();
      Renderer::translate(Vec3<float>(-10, 0, 0));
      Renderer::use_texture(m_textures_[SMILEY]);
      Renderer::draw_mesh(m_tile_mesh_);
    Renderer::pop_matrix();

    Renderer::push_matrix();
      Renderer::rotate(Vec3<float>(0, 0, static_cast<float>(glfwGetTime())));
      Renderer::use_texture(m_textures_[TEST]);
      Renderer::draw_mesh(m_test_mesh_);
    Renderer::pop_matrix();
  }

}//jdb