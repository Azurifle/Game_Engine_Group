#include <stdafx.h>
#include "Renderer.hpp"
#include "Mesh.hpp"

namespace jdb
{
  // ___ public __________________________________________________________________________________

  Renderer Renderer::instance{};

  void Renderer::render_bg(const Vec3<float>& t_rgb)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(t_rgb.x, t_rgb.y, t_rgb.z, 1);
  }

  void Renderer::set_projection_orthogonal(Vec2<float> t_grid_size)
  {
    REQUIRE(t_grid_size.x > 1); REQUIRE(t_grid_size.y > 1);

    t_grid_size /= 2;
    instance.m_view_projection_ = Mat4::ortho(-t_grid_size.x, t_grid_size.x
      , -t_grid_size.y, t_grid_size.y, 1, -1);
  }

  void Renderer::set_draw_frame(const Vec2<int>& t_pos_on_window
    , const Vec2<int>& t_frame_size)
  {
    REQUIRE(t_frame_size.x > 1); REQUIRE(t_frame_size.y > 1);

    glViewport(t_pos_on_window.x, t_pos_on_window.y, t_frame_size.x, t_frame_size.y);
  }

  void Renderer::push_matrix()
  {
    instance.m_matrices_.push_back(Mat4::identity());
  }

  void Renderer::pop_matrix()
  {
    REQUIRE(!instance.m_matrices_.empty());
    instance.m_matrices_.pop_back();
  }

  void Renderer::scale(const Vec3<float>& t_xyz)
  {
    instance.m_matrices_.back() *= Mat4::scaling(t_xyz);
  }

  void Renderer::rotate(const Vec3<float>& t_radian_angles)
  {
    instance.m_matrices_.back() *= Mat4::rotation(t_radian_angles);
  }

  void Renderer::translate(const Vec3<float>& t_xyz)
  {
    instance.m_matrices_.back() *= Mat4::translation(t_xyz);
  }

  void Renderer::use_texture(const Texture t_id)
  {
    glBindTexture(GL_TEXTURE_2D, t_id);
  }

  void Renderer::draw_mesh(const std::shared_ptr<Mesh>& t_mesh)
  {
    REQUIRE(t_mesh);

    glUseProgram(t_mesh->shader());

    auto model_transform = Mat4::identity();
    for(const auto MATRIX : instance.m_matrices_)
    {
      model_transform *= MATRIX;
    }
    const auto MVP_ARRAY = (model_transform * instance.m_view_projection_).to_array();
    glUniformMatrix4fv(t_mesh->vram_mvp(), 1, GL_FALSE, MVP_ARRAY);

    glBindVertexArray(t_mesh->id());
    glDrawArrays(t_mesh->draw_mode(), 0, t_mesh->vertices_count());
    delete[] MVP_ARRAY;
  }

  // ___ private ________________________________________________________________________________

  Renderer::Renderer(): m_view_projection_(Mat4::ortho(-100, 100, -100, 100, 1, -1)) {}

}//jdb