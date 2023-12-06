//
// sounds.cpp
//
#include "sounds.h"
#include "SDL2/SDL_mixer.h"


Sounds::Sounds()
{
  //  sound = NULL;
}


Sounds::~Sounds()
{
  //  sound = NULL;
}


bool Sounds::load(std::vector<std::string> fileNames)
{
  for (std::string  fileName : fileNames)
    {
      Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());

      if (sound == NULL)
        {
          SDL_Log("Failed to load sound: %s\nSDL_Mixer Error: %s"
                  , fileName.c_str(), Mix_GetError());
          return false;
        }
      sounds.push_back(sound);
    }

  fileNames.clear();
  
  return true;  
}


void Sounds::free()
{
  for (Mix_Chunk* sound : sounds)
    {
      Mix_FreeChunk(sound);
    }

  sounds.clear();
}


int Sounds::play(int sound, int loops)
{
  // TODO implement some kind of  boundary checking ala 
  // if (sound == NULL)
  //   {
  //     return -1;
  //   }
  
  // if (trysound > 7)
  //   trysound = 0;
  // int playedSound = trysound;

  // if (sound != -1)
  //   {
  //     playedSound = sound;
  //   }
  
  //SDL_Log("Played Sound: %d", playedSound);
  
  channel = Mix_PlayChannel(-1, sounds[sound], loops);
  
  return channel;
}


void Sounds::stop(int sound)
{
  //?? Might be better implemented with: Mix_ChannelFinished

  // check to make sure sound is still playing in the channel so we don't fade
  // out or stop a different sound in this channel
  if (Mix_GetChunk(channel) == sounds[sound])
    {
      // fade out or stop music
      Mix_FadeOutChannel(channel, 200);
    }
}


void Sounds::fade(int sound, int milliSecs)
{
  // check to make sure sound is still playing in the channel so we don't fade
  // out or stop a different sound in this channel
  if (Mix_GetChunk(channel) == sounds[sound])
    {
      // fade out or stop music
      Mix_FadeOutChannel(channel, milliSecs);
    }
}

