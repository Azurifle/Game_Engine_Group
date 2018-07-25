#include <stdafx.h>
#include "Mesh_factory.hpp"
#include "Mesh.hpp"

namespace jdb
{
  // ___ public __________________________________________________________________________________

  void Mesh_factory::new_mesh(const GLenum t_draw_mode, const Shader t_shader)
  {
    m_vertices_.clear();
    m_draw_mode_ = t_draw_mode;
    m_shader_ = t_shader;
  }

  void Mesh_factory::add_vertex(const Vec3<float>& t_pos, const Vec2<float>& t_texture_xy)
  {
    m_vertices_.push_back(Vertex(t_pos, t_texture_xy));
  }

  std::unique_ptr<Mesh> Mesh_factory::save_mesh()
  {
    REQUIRE(!m_vertices_.empty());

    auto temp = std::make_unique<Mesh>(
      m_draw_mode_, m_shader_, m_vertices_);

    m_vertices_.clear();

    return std::move(temp);
  }
}//jdb