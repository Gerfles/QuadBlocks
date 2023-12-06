//
// sounds.h
//
#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <string>
#include <vector>
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_log.h"
//#include "audio.h"

class Sounds
{
private:
  std::vector<Mix_Chunk*> sounds;
  int channel;
public:
  Sounds();
  ~Sounds();
  
  bool load(std::vector<std::string> fileNames);
  void free();
  int play(int sound, int loops = 0);
  void stop(int sound);
  void fade(int sound, int milliSecs = 300);
  bool isLoaded() { return (sounds.empty()); }
};
#endif 
