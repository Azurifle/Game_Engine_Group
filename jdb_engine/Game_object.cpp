#include "stdafx.h"
#include "Game_object.hpp"
#include "visual/Renderer.hpp"

namespace jdb
{
  // ___ public _________________________________________________________________________________

  Game_object::Game_object()
    : pos(Vec3<float>(0)), radian_angles(Vec3<float>(0)), scale(Vec3<float>(1)) {}

  void Game_object::render() const
  {
    Renderer::push_matrix();
      Renderer::scale(scale);
      Renderer::rotate(radian_angles);
      Renderer::translate(pos);

      Renderer::use_texture(texture);
      Renderer::draw_mesh(mesh);
    Renderer::pop_matrix();
  }
}//jdb