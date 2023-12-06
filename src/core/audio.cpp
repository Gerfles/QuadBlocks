//
// audio.cpp
//

#include "audio.h"

bool Audio::init()
{
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
		    2, 1024) == -1)
    {
      SDL_Log("Failed to initialize audio!");
      return false;
    }

  // allocate mixing channels -- this turns out to be really important as the defualt
  // amount of channels seems to be pretty low ~8 or so!!
  Mix_AllocateChannels(64);
  
  return true;
}

void Audio::setMusic(Music* musicTracks)
{
  music = musicTracks;
}

void Audio::setSounds(Sounds* soundBank)
{
  sounds = soundBank;
}


void Audio::kill()
{
  Mix_CloseAudio();
}


void Audio::pauseMusic()
{
  Mix_PauseMusic();
}


void Audio::resumeMusic()
{
  Mix_ResumeMusic();
}


void Audio::stopMusic()
{
  Mix_HaltMusic();
}


void Audio::stopChannel( int channel )
{
  Mix_HaltChannel(channel);
}
