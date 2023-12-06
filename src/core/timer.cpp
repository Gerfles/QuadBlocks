// timer.cpp
//
// game timer class

#include "timer.h"
#include "SDL2/SDL_timer.h"


Timer::Timer()
{
  mStartTicks = 0;
  mPausedTicks = 0;
  mIsPaused = false;
  mHasStarted = false;
}


void Timer::start()
{
  // start the timer
  mHasStarted = true;

  // make sure the timer is not paused
  mIsPaused = false;

  // save the current clock time to startTicks
  mStartTicks = SDL_GetTicks();

  // reset paused time 
  mPausedTicks = 0;
}


void Timer::stop()
{
  // stop the timer
  mHasStarted = false;

  // make sure the timer is not paused
  mIsPaused = false;

  // reset time to zero
  mStartTicks = 0;
  mPausedTicks = 0;
}


void Timer::pause()
{
  // check if the timer is running and unpaused
  if (mHasStarted && !mIsPaused)
    {
      // Pause the timer
      mIsPaused = true;

      // set the paused time to the current elapsed time
      mPausedTicks = SDL_GetTicks() - mStartTicks;
      //?? reset the start time -- not sure it's needed
      mStartTicks = 0;
    }

}


void Timer::unPause()
{
  // make sure that the timer has in fact started and been paused
  if (mHasStarted && mIsPaused)
    {
      // unpause the timer
      mIsPaused = false;

      // reset the starting ticks
      mStartTicks = SDL_GetTicks() - mPausedTicks;

      // reset the paused ticks
      mPausedTicks = 0;
    }
}


Uint32 Timer::getTicks()
{
  // set the default return time 
  Uint32 time = 0;
  
  // if the timer has been started, return the internally stored time
  if (mHasStarted)
    {
      // return the paused time if timer is not running
      if (mIsPaused)
        {
          time = mPausedTicks;
        }
      else // return the time since timer started
        {
          time = (SDL_GetTicks() - mStartTicks);
        }
    }
  
  return time;
}

bool Timer::hasStarted()
{
  // timer is running and either paused or unpaused
  return mHasStarted;
}


bool Timer::isPaused()
{
  // timer is running and paused
  return (mIsPaused && mHasStarted);
}
