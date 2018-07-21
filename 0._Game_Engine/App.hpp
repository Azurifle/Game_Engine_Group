#ifndef G6037599_APP_HPP
#define G6037599_APP_HPP
#pragma once
#include "My_Math/Mat4.hpp"
#include "Visual_Manager/Mesh_renderer.hpp"
#include "Visual_Manager/Texture_manager.hpp"
#include <vector>

//#include "Vertex.h"
namespace jdb
{
	/*class App final
	{
	private:

		GLFWwindow *window = nullptr;
		std::vector<Vertex> vertices;

		GLuint programID;
		GLuint vertexArrayID;
		GLint mvp_location;
		GLuint textureID;

		int width;
		int height;
	public:
		App();
		~App();
		void Start();
		void init(int WIDTH, int HEIGHT, const std::string& tittle);
		void initMesh();
		void initShaderProgram();
		void initTexture();

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
	};*/
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
    static const std::string TEXTURE_PATH;

    GLFWwindow* m_window_{};
    Texture m_texture_id_{};
    int m_width_, m_height_;
    bool m_is_active_;
    Mesh_renderer m_mesh_renderer_{};

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

#endif //G6037599_APP_HPP
