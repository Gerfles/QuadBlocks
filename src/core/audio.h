//
// audio.h
//
#ifndef AUDIO_H_
#define AUDIO_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "music.h"
#include "sounds.h"


class Audio
{
private:
public:
  Music* music;
  Sounds* sounds;
  bool init();
  void kill();
  
  // music funtions
  void setMusic(Music* music);
  void setSounds(Sounds* sounds);
  // TODO figure out which member functions to remove 
  void pauseMusic();
  void resumeMusic();
  void stopMusic();

  
  void stopChannel(int channel);
  bool musicPlaying() { return Mix_PlayingMusic(); }
  bool musicPuased() { return Mix_PausedMusic(); }
};
#endif
