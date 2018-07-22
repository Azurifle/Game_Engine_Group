#include <stdafx.h>
#include "App.hpp"
#include "visual/Vertex.hpp"
#include "my_math/Mat4.hpp"
#include "visual/Shader.hpp"
#include "visual/Renderer.hpp"
#include "visual/Mesh.hpp"
#include "visual/Texture_manager.hpp"
#include "Engine.hpp"

namespace jdb
{
  //adapt from https://github.com/redeian/FirstGLFW/tree/master/FirstGLFW

  // ___ (de)constructors ________________________________________________

  App::App(const int t_width, const int t_height, const std::string& t_title) 
    : m_width_(t_width), m_height_(t_height), m_is_active_(true)
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

    static const std::string VERTEX_SHADER_CODE =
      "uniform mat4 MVP; \n"	//MVP is Model View Projection
      "attribute vec3 vPos; \n"
      "attribute vec3 vCol; \n"
      "attribute vec2 vTex; \n"
      "varying vec3 color; \n"
      "varying vec2 texCoord; \n"
      "void main() \n "
      "{ \n"
      "   gl_Position = MVP * vec4(vPos, 1.0); \n"
      "   color = vCol; \n"
      "   texCoord = vTex; \n"
      "} \n"

      , FRAGMENT_SHADER_CODE = //color & texture
      "uniform sampler2D texture; \n"
      "varying vec3 color; \n"
      "varying vec2 texCoord; \n"
      "void main() \n"
      "{ \n"
      "    gl_FragColor = texture2D( texture, texCoord);\n"
      "} \n";

    Renderer::use_shader(Shader::create(VERTEX_SHADER_CODE, FRAGMENT_SHADER_CODE));//load_or_get(t_folder_path)
    m_texture_id_ = Texture_manager::get().load_or_get(GAME_FOLDER+"texture/Test.png");
    m_texture_tile_id_ = Texture_manager::get().load_or_get(GAME_FOLDER + "texture/Grass.png", GL_RGB);

    const auto GREEN = Vec3<float>(0, 1, 0), BLUE = Vec3<float>(0, 0, 1)
      , RED = Vec3<float>(1, 0, 0);
    Mesh mesh{};
    mesh.add_vertex(Vec3<float>(0.6f, -0.4f), BLUE, Vec2<float>(1, 1));//pos rb, tex lb
    mesh.add_vertex(Vec3<float>(-0.6f, -0.4f), GREEN, Vec2<float>(0, 1));//pos lb, tex lb
    mesh.add_vertex(Vec3<float>(0, 0.6f), RED, Vec2<float>(0.5f, 0));//pos mt, tex mt

    m_mesh_renderer_ = Mesh_renderer(mesh);

    mesh = Mesh();
    static const auto TILE_SIZE = 0.2f;
    mesh.add_vertex(Vec3<float>(-TILE_SIZE, TILE_SIZE), BLUE, Vec2<float>(0, 0));//lt
    mesh.add_vertex(Vec3<float>(TILE_SIZE, TILE_SIZE), GREEN, Vec2<float>(1, 0));//rt
    mesh.add_vertex(Vec3<float>(TILE_SIZE, -TILE_SIZE), RED, Vec2<float>(1, 1));//rb
    mesh.add_vertex(Vec3<float>(-TILE_SIZE, -TILE_SIZE), Engine::WHITE, Vec2<float>(0, 1));//lb
    m_tile_mesh_renderer_ = Mesh_renderer(mesh);
  }

  App::~App()
  {
    glfwTerminate();
    Texture_manager::get().unload_all();
    puts("App destroyed.");
  }

  // ___ public ________________________________________________

  void App::run() const
  {
    while (!glfwWindowShouldClose(m_window_))
    {
      render_background();//Renderer::render_background(t_rgb);********
      glViewport(0, 0, m_width_, m_height_);//Renderer::set_projection(Renderer::PERSPECTIVE); in constructor
      switch (m_is_active_) { case true: render_objects(); default:; }
      glfwSwapBuffers(m_window_);
      glfwPollEvents();//clear inputs
    }
  }

  // ___ private ________________________________________________

  const std::string App::GAME_FOLDER = "shadow_maze/";

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

  void App::render_background()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const Vec3<float> BG_BLUE_GREEN(0, 0.3f, 0.8f);
    glClearColor(BG_BLUE_GREEN.x, BG_BLUE_GREEN.y, BG_BLUE_GREEN.z, 1);
  }

  void App::init_glfw_system(const std::string& t_title)
  {
    switch (glfwInit())
    {
    case GLFW_FALSE: glfwTerminate(); PROMISE(false); default:;
    }

    m_window_ = glfwCreateWindow(m_width_, m_height_, t_title.c_str(), nullptr, nullptr);
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
    Renderer::use_texture(m_texture_id_);//Texture_manager::get().load_or_get(GAME_FOLDER)
    
    //Renderer::push_matrix (m_matrices_.push_back())
    Mat4 MVP;

    //Renderer::rotate(Vec3<float>(0, 0, 1));
    model_view_projection(MVP);

    //Renderer::draw_mesh(m_mesh_renderer_);
    const auto MVP_ARRAY = MVP.to_array();
    glUniformMatrix4fv(m_mesh_renderer_.mvp_location(), 1, GL_FALSE, MVP_ARRAY);
    glBindVertexArray(m_mesh_renderer_.id());
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //Renderer::pop_matrix (m_matrices_.pop_back())
    delete[] MVP_ARRAY;
    

    //render a tile
    Renderer::use_texture(m_texture_tile_id_);//Texture_manager::get().load_or_get(GAME_FOLDER)

    const auto RATIO = m_width_ / static_cast<float>(m_height_);
    const auto MODEL_MAT = Mat4::translation(Vec3<float>(-1, 0, 0))
      , VIEW_PROJECTION = Mat4::ortho(-RATIO, RATIO, -1, 1, 1, -1);
    auto mvp_tile = MODEL_MAT * VIEW_PROJECTION;

    const auto MVP_ARRAY2 = mvp_tile.to_array();
    glUniformMatrix4fv(m_tile_mesh_renderer_.mvp_location(), 1, GL_FALSE, MVP_ARRAY2);
    glBindVertexArray(m_tile_mesh_renderer_.id());
    glDrawArrays(GL_QUADS, 0, 4);

    delete[] MVP_ARRAY2;
  }

  void App::model_view_projection(Mat4& t_out_mvp) const
  {
    const auto ANGLE = static_cast<float>(glfwGetTime())
      , RATIO = m_width_ / static_cast<float>(m_height_);
    const auto ROTATE_MAT = Mat4::rotation(Vec3<float>(0, 0, 0))//ANGLE, Vec3<int>(0, 0, 1)
      , MODEL_MAT = Mat4::translation(Vec3<float>(0)) * ROTATE_MAT * Mat4::scaling(Vec3<float>(1))
      , VIEW_PROJECTION = Mat4::ortho(-RATIO, RATIO, -1, 1, 1, -1);//Renderer::set_projection(Renderer::PERSPECTIVE); in App constructor
    t_out_mvp = MODEL_MAT * VIEW_PROJECTION;
  }

}//jdb