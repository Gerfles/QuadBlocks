//
// music.h
//
#ifndef MUSIC_H_
#define MUSIC_H_

#include <string>
#include <vector>
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_log.h"

class Music
{
private:
  std::vector<Mix_Music*> music;
  std::vector<int> trackNumbers; //?? should this be a list instead
  int lastTrack;
  int currentTrack;

  
public:
  Music();
  ~Music(); 

  bool load(std::vector<std::string> fileNames);
  void free();
  void update();
  void playTrack(int track, int loops = 0);
  void playNext();
  void shuffleSongs(int* songPlaylist = NULL, int numElements = 0);
  bool isLoaded() { return (music.empty()); }
  void fade(int milliSecs);
};
#endif

