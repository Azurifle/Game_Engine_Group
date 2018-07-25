#ifndef JDB_SHADER_HPP
#define JDB_SHADER_HPP
#pragma once

namespace jdb
{
  using Shader = GLuint;

  class Shader_manager final
  {
  public:
    //Folder must contain file name "Vertex.shader" and "Fragment.shader"
    static Shader load_or_get(const std::string& t_folder_path = "jdb_engine/visual/jdk_texture_shader/");

    ~Shader_manager() = default;
  private:
    std::map<std::string, Shader> m_loadeds_{};
    Shader load(const std::string& t_folder_path);

    static void step1_create_shader_program(std::vector<Shader>& t_shader_ids);
    static void step2_load_shader_codes(const std::vector<Shader>& t_out_shader_ids
      , const std::string& t_folder_path);
    static void step4_check_compilation(Shader t_shader_program_id);
    static void step7_check_linking(Shader t_id, const std::string& t_folder_path);

    enum Message { LENGTH = 256 };
    static void show_compile_error(Shader t_shader_id);
    static void show_program_error(Shader t_id, const std::string& t_text);
    static const char* show_succeed_failed(GLint t_link_status);

    Shader_manager() = default;
    Shader_manager(const Shader_manager& t_to_copy) = default;
    Shader_manager(Shader_manager&& t_to_move) = default;
    Shader_manager& operator = (const Shader_manager& t_to_copy) = default;
    Shader_manager& operator = (Shader_manager&& t_to_move) noexcept = default;
  };
}//jdb

#endif //JDB_SHADER_HPP
