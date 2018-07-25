#include <stdafx.h>
#include "Shader_manager.hpp"
#include "jdb_engine/Engine.hpp"

namespace jdb
{
  // ___ public __________________________________________________________________________________

  Shader Shader_manager::load_or_get(const std::string& t_folder_path)
  {
    static Shader_manager instance;

    const auto FOUND_SHADER = instance.m_loadeds_.find(t_folder_path);
    if (FOUND_SHADER != instance.m_loadeds_.end())
    {
      return FOUND_SHADER->second;
    }

    return instance.load(t_folder_path);
  }

  // ___ private _________________________________________________________________________________

  Shader Shader_manager::load(const std::string& t_folder_path)
  {
    std::vector<Shader> ids;
    step1_create_shader_program(ids);
    step2_load_shader_codes(ids, t_folder_path);

    enum Id_type { PROGRAM, VERTEX, FRAGMENT };
    glCompileShader(ids[VERTEX]);
    glCompileShader(ids[FRAGMENT]);

    step4_check_compilation(ids[VERTEX]);
    step4_check_compilation(ids[FRAGMENT]);

    glAttachShader(ids[PROGRAM], ids[VERTEX]);
    glAttachShader(ids[PROGRAM], ids[FRAGMENT]);

    glLinkProgram(ids[PROGRAM]);
    step7_check_linking(ids[PROGRAM], t_folder_path);

    m_loadeds_[t_folder_path] = ids[PROGRAM];
    return ids[PROGRAM];
  }

  void Shader_manager::step1_create_shader_program(std::vector<Shader>& t_shader_ids)
  {
    t_shader_ids.push_back(glCreateProgram());
    t_shader_ids.push_back(glCreateShader(GL_VERTEX_SHADER));
    t_shader_ids.push_back(glCreateShader(GL_FRAGMENT_SHADER));
  }

  void Shader_manager::step2_load_shader_codes(const std::vector<Shader>& t_out_shader_ids
    , const std::string& t_folder_path)
  {
    std::string vertex_shader_code, fragment_shader_code;
    Engine::load_txt(t_folder_path + "Vertex.shader", vertex_shader_code);
    Engine::load_txt(t_folder_path + "Fragment.shader", fragment_shader_code);

    auto VERTEX_CODE = vertex_shader_code.c_str()
      , FRAGMENT_CODE = fragment_shader_code.c_str();

    glShaderSource(t_out_shader_ids[1], 1, &VERTEX_CODE, nullptr);
    glShaderSource(t_out_shader_ids[2], 1, &FRAGMENT_CODE, nullptr);
  }

  void Shader_manager::step4_check_compilation(const Shader t_shader_id)
  {
    GLint result;
    glGetShaderiv(t_shader_id, GL_COMPILE_STATUS, &result);
    switch (result) { case GL_TRUE: return; default:; }
    show_compile_error(t_shader_id);
  }

  void Shader_manager::step7_check_linking(const Shader t_id, const std::string& t_folder_path)
  {
    GLint link_status, validate_status;

    glGetProgramiv(t_id, GL_LINK_STATUS, &link_status);
    switch (link_status)
    {
    case GL_FALSE: show_program_error(t_id, "Linking"); default:;
    }

    glValidateProgram(t_id);
    glGetProgramiv(t_id, GL_VALIDATE_STATUS, &validate_status);
    switch (validate_status)
    {
    case GL_FALSE: show_program_error(t_id, "Validation"); default:;
    }

    std::cout << "Shader \"" << t_folder_path << "\" Link: " << show_succeed_failed(link_status)
      << ", Validate: " << show_succeed_failed(validate_status) << std::endl;
  }

  void Shader_manager::show_compile_error(const Shader t_id)
  {
    std::cout << "Shader compilation FAILED" << std::endl;
    GLchar messages[LENGTH];

    glGetShaderInfoLog(t_id, sizeof messages, nullptr, &messages[0]);
    std::cout << messages;

    glfwTerminate();
    PROMISE(false);
  }

  void Shader_manager::show_program_error(const Shader t_id, const std::string& t_text)
  {
    std::cout << "Shader " << t_text << " FAILED" << std::endl;
    GLchar messages[LENGTH];

    glGetProgramInfoLog(t_id, sizeof messages, nullptr, &messages[0]);
    std::cout << messages;

    glfwTerminate();
    PROMISE(false);
  }

  const char* Shader_manager::show_succeed_failed(const GLint t_link_status)
  {
    return t_link_status ? "succeed" : "FAILED";
  }

}//jdb