//
// music.cpp
//
// flexible music class that can load all the songs for the full game and only
// play those passed in via a tracklist array or can load only the songs needed
// for the current game state by using a music class in each gamestate
#include "music.h"
#include "SDL2/SDL_mixer.h"
#include <algorithm>
#include <functional>
// #include <type_traits>


Music::Music()
{
  //  music = NULL;
}


Music::~Music()
{
  //  music = NULL;
}


bool Music::load( std::vector<std::string> fileNames )
{
  int i = 0;

  for (std::string fileName : fileNames)
    {
      Mix_Music* track = Mix_LoadMUS(fileName.c_str());

      if (track == NULL)
        {
          SDL_Log("Failed to load song: %s", fileName.c_str());
          return false;
        }
      trackNumbers.push_back(i++);
      music.push_back(track);
    }

  // empty filenames here but would have to make sure user doesn't plan on re-loading
  fileNames.clear();

  // set the playlist length to the full vector for now
  lastTrack = music.size();
  currentTrack = 0;

  // FAILED ATTEMPTS TO GET MIX_HOOKMUsicfinished WORKING CORRECTLY 
  // use musicFinished for when music stops
  // Mix_HookMusicFinished(playNext());
  // pt2function pn = &Music::playNext();
  // pt2MF fun = playNext;
  // auto lambdafun = [](){SDL_Log("Lambda");};
  // int num_callbacks=0;
  // auto callback=[&](){SDL_Log("callback called %d times", num_callbacks);};
  // auto thunk=[](void* arg){ // note thunk is captureless
  // (*static_cast<decltype(callback)*>(arg))();};
  // std::function<void()> attempted = &Music::test;
  // std::bind(test, this); //&boundfn;
  // Mix_HookMusicFinished(lambdafun);
  // Mix_HookMusicFinished(&test);
  
  return true;
}


void Music::free()
{
  for (Mix_Music* track : music)
    {
      // free each song loaded
      Mix_FreeMusic(track);
    }
  // empty the contents of music tracks vector
  music.clear();
  trackNumbers.clear();
}


void Music::playTrack(int track, int loops)
{
  // make sure music is not empty and track is within bounds
  if (track >= 0 && track < music.size())
    {
      Mix_PlayMusic(music[track], loops);
    }
}


void Music::update()
{
  if (!Mix_PlayingMusic())
    {
      playNext();
    }
}


void Music::playNext()
{
  // only play the tracks we're allowed to
  if (currentTrack < lastTrack)
    {
      // play the next track in music vector according to the ordering in tracknumbers
      Mix_PlayMusic(music[trackNumbers[currentTrack++]], 0);
    }
  else
    {
      Mix_PlayMusic(music[trackNumbers[0]], 0);
      currentTrack = 1;
    }
}


void Music::shuffleSongs(int* songPlaylist, int numElements)
{
  //FIXME
  if (1)//Mix_PlayingMusic())
    {
      Mix_HaltMusic();
    }
  // if user doesn't pass a playlist of specific songs
  if (songPlaylist == NULL)
    { // shuffle all songs but not actually shuffle the collection to make sure it stays in order
      std::random_shuffle(trackNumbers.begin(), trackNumbers.end());
    }
  else // only shuffle the songs passed in the songplaylist and place them at the front of the vector
    {
      for (int i = 0; i < numElements; i++)
        {
          // pick a random track number from the passed playlist array
          int track = rand() % numElements;

          // make sure we don't pick a track that's already been selected
          while(songPlaylist[track] == -1)
            {
              track = rand() % numElements;
            }
          
          // swap that track out to with the first i element in the track number list
          std::swap(trackNumbers[songPlaylist[track]], trackNumbers[i]);

          // set the chosen track to -1 so we can avoid selecting the same track again
          songPlaylist[track] = -1;
        }
      // set the terminating song of the playlist so that we don't play any further into it 
      lastTrack = numElements;
    }
  // reset our position in the playlist to the beginning of the vector
  currentTrack = 0;
}


void Music::fade(int milliSecs)
{
  Mix_FadeOutMusic(milliSecs);
}
