// timer.h
//
// game timer class

#ifndef TIMER_H_
#define TIMER_H_

#include "SDL2/SDL.h"

class Timer
{
private:
  // the application clock time (started when application starts) when the timer started
  Uint32 mStartTicks;

  // stores the ticks if timer gets paused
  Uint32 mPausedTicks;
  
  // timer state
  bool mIsPaused;
  bool mHasStarted;
  
public:
  // initialize variables
  Timer();

  // clock actions
  void start();
  void stop();
  void pause();
  void unPause();

  // getters
  Uint32 getTicks();
  bool isPaused();
  bool hasStarted();
};


#endif
  
