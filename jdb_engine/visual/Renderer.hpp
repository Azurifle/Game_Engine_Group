#ifndef JDB_RENDERER_HPP
#define JDB_RENDERER_HPP
#pragma once
#include "Texture_manager.hpp"
#include "../my_math/Vec2.hpp"
#include "../my_math/Vec3.hpp"
#include "../my_math/Mat4.hpp"

namespace jdb
{
  class Mesh;

  class Renderer final
  {
  public:
    static Renderer instance;

    static void render_bg(const Vec3<float>& t_rgb);
    static void set_projection_orthogonal(Vec2<int> t_grid_size);

    //default is set_draw_frame(0, 0, window_size.x, window_size.y) = full window
    //(0, 0) means left bottom edge of the window
    //
    //Useful for drawing a mini map
    static void set_draw_frame(const Vec2<int>& t_pos_on_window, const Vec2<int>& t_frame_size);

    //multiply first do first, in world coordinate
    static void push_matrix();
    static void pop_matrix();
    static void scale(const Vec3<float>& t_xyz);
    static void rotate(const Vec3<float>& t_radian_angles);
    static void translate(const Vec3<float>& t_xyz);
    static void use_texture(Texture t_id);

    //draw last stay on top
    static void draw_mesh(const std::shared_ptr<Mesh>& t_mesh);

    ~Renderer() = default;
  private:
    Mat4 m_view_projection_;
    std::vector<Mat4> m_matrices_{};

    Renderer();
    Renderer(const Renderer& t_to_copy) = default;
    Renderer(Renderer&& t_to_move) noexcept = default;
    Renderer& operator = (const Renderer& t_to_copy) = default;
    Renderer& operator = (Renderer&& t_to_move) noexcept = default;
  };
}//jdb

#endif JDB_RENDERER_HPP;