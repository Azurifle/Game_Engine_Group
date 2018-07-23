#ifndef JDB_APP_HPP
#define JDB_APP_HPP
#pragma once
#include "visual/Texture_manager.hpp"
#include "visual/Mesh.hpp"
#include "my_math/Vec2.hpp"

namespace jdb
{
  struct Vertex;

  class App final
  {
  public:
    App(const Vec2<int>& t_size, const std::string& t_title);
    ~App();
    App(const App& t_to_copy) = default;
    App(App&& t_to_move) noexcept = default;
    App& operator = (const App& t_to_copy) = default;
    App& operator = (App&& t_to_move) noexcept = default;

    void run() const;
  private:
    static const std::string TEXTURE_FOLDER;

    std::vector<Texture> m_textures_{};
    Vec2<int> m_size_;
    GLFWwindow* m_window_{};
    bool m_is_active_;
    Mesh m_test_mesh_{}, m_tile_mesh_{};

    void init_glfw_system(const std::string& t_title);
    static void key_callback_static(GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods);
    void key_callback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods);
    static void show_glew_versions();
    static void test_depth_n_alpha_bend();
    void render_objects() const;
  };
}//jdb

#endif //JDB_APP_HPP
