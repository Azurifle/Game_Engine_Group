#ifndef G6037599_AUDIO_MANAGER_HPP
#define G6037599_AUDIO_MANAGER_HPP
#pragma once

namespace jdb
{
  using Audio = ALuint;

  class Audio_manager final
  {
  public:
    static Audio load_or_get_audio(const std::string& t_wav_file_path);
    static void play(Audio t_audio);
    static void pause(Audio t_audio);
    static void stop(Audio t_audio);

    ~Audio_manager();
  private:
    static Audio load(const std::string& t_wav_file_path
      , std::map<std::string, Audio>& t_audio_collection);
    static void check_load_sound_error(ALuint t_buffer);
    static void check_play_sound(ALenum t_error);

    std::map<std::string, Audio> m_audio_collection_{};

    Audio_manager();
    Audio_manager(const Audio_manager& t_to_copy) = default;
    Audio_manager(Audio_manager&& t_to_move) = default;
    Audio_manager& operator = (const Audio_manager& t_to_copy) = default;
    Audio_manager& operator = (Audio_manager&& t_to_move) noexcept = default;
  };
}//jdb

#endif //G6037599_AUDIO_MANAGER_HPP
