#ifndef JDB_APP_HPP
#define JDB_APP_HPP
#pragma once
#include "my_math/Mat4.hpp"
#include "visual/Mesh_renderer.hpp"
#include "visual/Texture_manager.hpp"

namespace jdb
{
  struct Vertex;

  class App final
  {
  public:
    App(int t_width, int t_height, const std::string& t_title);
    ~App();
    App(const App& t_to_copy) = default;
    App(App&& t_to_move) noexcept = default;
    App& operator = (const App& t_to_copy) = default;
    App& operator = (App&& t_to_move) noexcept = default;

    void run() const;
  private:
    static const std::string TEXTURE_FOLDER;

    GLFWwindow* m_window_{};
    std::vector<Texture> m_textures_{};
    int m_width_, m_height_;
    bool m_is_active_;
    Mesh_renderer m_mesh_renderer_{}, m_tile_mesh_renderer_{};

    void init_glfw_system(const std::string& t_title);
    static void key_callback_static(GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods);
    void key_callback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods);
    static void show_glew_versions();
    static void test_depth_n_alpha_bend();
    static void render_background();
    void render_objects() const;
    void model_view_projection(Mat4& t_out_mvp) const;
  };
}//jdb

#endif //JDB_APP_HPP
