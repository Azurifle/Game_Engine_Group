#include <stdafx.h>
#include "Audio_manager.hpp"

namespace jdb
{
  //___ public __________________________________________________________________________________

  Audio Audio_manager::load_or_get_audio(const std::string& t_wav_file_path)
  {
    static Audio_manager instance;

    const auto FOUND_AUDIO = instance.m_audio_collection_.find(t_wav_file_path);
    if (FOUND_AUDIO != instance.m_audio_collection_.end())
    {
      return FOUND_AUDIO->second;
    }

    return load(t_wav_file_path, instance.m_audio_collection_);
  }

  void Audio_manager::play(const Audio t_audio)
  {
    alSourcePlay(t_audio);//on other process
    check_play_sound(alGetError());
  }

  void Audio_manager::pause(const Audio t_audio)
  {
    alSourcePause(t_audio);
    check_play_sound(alGetError());
  }

  void Audio_manager::stop(const Audio t_audio)
  {
    alSourceStop(t_audio);
    check_play_sound(alGetError());
  }

  //___ deconstructor ____________________________________________________________________________

  Audio_manager::~Audio_manager()
  {
    switch (alutExit())
    {
    case false: fprintf(stderr, "%s\n", alutGetErrorString(alutGetError()));
      PROMISE(false); default:;
    }
    puts("Audio_manager destroyed.");
  }

  //___ private __________________________________________________________________________________

  Audio Audio_manager::load(const std::string& t_wav_file_path
    , std::map<std::string, Audio>& t_audio_collection)
  {
    REQUIRE(!t_wav_file_path.empty());
    const auto AL_BUFFER_ID = alutCreateBufferFromFile(t_wav_file_path.c_str());
    check_load_sound_error(AL_BUFFER_ID);

    ALuint audio_source_id;
    alGenSources(1, &audio_source_id);
    alSourcei(audio_source_id, AL_BUFFER, AL_BUFFER_ID);//attach the buffer to it

    t_audio_collection[t_wav_file_path] = audio_source_id;

    return audio_source_id;
  }

  void Audio_manager::check_load_sound_error(const ALuint t_buffer)
  {
    switch (t_buffer)
    {
    case AL_NONE: break;
    default: return;
    }

    fprintf(stderr, "Error loading file: '%s'\n", alutGetErrorString(alutGetError()));
    alutExit();
    PROMISE(false);
  }

  void Audio_manager::check_play_sound(const ALenum t_error)
  {
    switch (t_error) { case ALUT_ERROR_NO_ERROR: return; default:; }

    fprintf(stderr, "%s\n", alGetString(t_error));
    alutExit();
    PROMISE(false);
  }

  //___ private constructor ______________________________________________________________________

  Audio_manager::Audio_manager()
  {
    alutInit(nullptr, nullptr);
    puts("Audio_manager initialized.");
  }
  }//jdb