#include <stdafx.h>
#include "Mesh_manager.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"

namespace jdb
{
  // ___ public __________________________________________________________________________________

  Mesh_manager Mesh_manager::instance{};

  void Mesh_manager::new_mesh(const GLenum t_draw_mode, const Shader t_shader)
  {
    instance.m_vertices_.clear();
    instance.m_draw_mode_ = t_draw_mode;
    instance.m_shader_ = t_shader;
  }

  void Mesh_manager::add_vertex(const Vec3<float>& t_pos, const Vec2<float>& t_texture_xy)
  {
    instance.m_vertices_.push_back(Vertex(t_pos, t_texture_xy));
  }

  Mesh&& Mesh_manager::save_mesh()
  {
    return std::move(Mesh(instance.m_draw_mode_, instance.m_shader_, instance.m_vertices_));
  }
}//jdb