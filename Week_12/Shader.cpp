#include <stdafx.h>
#include "Shader.hpp"

namespace G6037599
{
  // ___ constructor ___________________________________________________________________________________________

  GLuint Shader::create(const std::string& t_vertex_shader_code, const std::string& t_fragment_shader_code)
  {
    std::vector<GLuint> shader_ids;
    step1_create_shader_program(shader_ids);
    step2_load_shader_codes(shader_ids, t_vertex_shader_code, t_fragment_shader_code);

    enum Enum { PROGRAM, VERTEX, FRAGMENT };
    glCompileShader(shader_ids[VERTEX]);
    glCompileShader(shader_ids[FRAGMENT]);

    step4_check_compilation(shader_ids[VERTEX]);
    step4_check_compilation(shader_ids[FRAGMENT]);

    glAttachShader(shader_ids[PROGRAM], shader_ids[VERTEX]);
    glAttachShader(shader_ids[PROGRAM], shader_ids[FRAGMENT]);

    glLinkProgram(shader_ids[PROGRAM]);
    step7_check_linking(shader_ids[PROGRAM]);
    return shader_ids[PROGRAM];
  }

  // ___ private static _______________________________________________________________________________________

  void Shader::step1_create_shader_program(std::vector<GLuint>& t_shader_ids)
  {
    t_shader_ids.push_back(glCreateProgram());
    t_shader_ids.push_back(glCreateShader(GL_VERTEX_SHADER));
    t_shader_ids.push_back(glCreateShader(GL_FRAGMENT_SHADER));
  }

  void Shader::step2_load_shader_codes(const std::vector<GLuint>& t_out_shader_ids
    , const std::string& t_vertex_shader_code, const std::string& t_fragment_shader_code)
  {
    auto VERTEX_CODE = t_vertex_shader_code.c_str(), FRAGMENT_CODE = t_fragment_shader_code.c_str();

    glShaderSource(t_out_shader_ids[1], 1, &VERTEX_CODE, nullptr);
    glShaderSource(t_out_shader_ids[2], 1, &FRAGMENT_CODE, nullptr);
  }

  void Shader::step4_check_compilation(const GLuint t_shader_id)
  {
    GLint result;
    glGetShaderiv(t_shader_id, GL_COMPILE_STATUS, &result);
    switch (result) { case GL_TRUE: return; default:; }
    show_shader_error(t_shader_id, "Compilation", false);
  }

  void Shader::step7_check_linking(const GLuint t_m_program_id)
  {
    GLint link_status, validate_status;

    glGetProgramiv(t_m_program_id, GL_LINK_STATUS, &link_status);
    switch (link_status)
    {
    case GL_FALSE: show_shader_error(t_m_program_id, "Linking"); default:;
    }

    glValidateProgram(t_m_program_id);
    glGetProgramiv(t_m_program_id, GL_VALIDATE_STATUS, &validate_status);
    switch (validate_status)
    {
    case GL_FALSE: show_shader_error(t_m_program_id, "Validation"); default:;
    }

    std::cout << "Link: " << link_status
      << ", Validate: " << validate_status << std::endl;
  }

  void Shader::show_shader_error(const GLuint t_shader_id
    , const std::string t_text, const bool t_is_program)
  {
    std::cout << "Shader " << t_text << " FAILED" << std::endl;
    const auto MESSAGE_LENGTH = 256;
    GLchar messages[MESSAGE_LENGTH];
    switch (t_is_program)
    {
    case false: glGetShaderInfoLog(t_shader_id, sizeof messages, nullptr, &messages[0]);
      break;
    default: glGetProgramInfoLog(t_shader_id, sizeof messages, nullptr, &messages[0]);
    }
    std::cout << messages;
    glfwTerminate();
    PROMISE(false);
  }

}//G6037599