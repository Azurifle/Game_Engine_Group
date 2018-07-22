#ifndef G6037599_SHADER_HPP
#define G6037599_SHADER_HPP
#pragma once

namespace jdb
{
  class Shader final
  {
  public:
    static GLuint create(const std::string& t_vertex_shader_code, const std::string& t_fragment_shader_code);
  private:
    static void step1_create_shader_program(std::vector<GLuint>& t_shader_ids);
    static void step2_load_shader_codes(const std::vector<GLuint>& t_out_shader_ids
      , const std::string& t_vertex_shader_code, const std::string& t_fragment_shader_code);
    static void step4_check_compilation(GLuint t_shader_program_id);
    static void step7_check_linking(GLuint t_m_program_id);
    static void show_shader_error(GLuint t_shader_id, std::string t_text, bool t_is_program = true);

    Shader() = default;
    ~Shader() = default;
    Shader(const Shader& t_to_copy) = default;
    Shader(Shader&& t_to_move) noexcept = default;
    Shader& operator = (const Shader& t_to_copy) = default;
    Shader& operator = (Shader&& t_to_move) noexcept = default;
  };
}//jdb

#endif //G6037599_SHADER_HPP
