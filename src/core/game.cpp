// game.cpp
//
// must compile with the following frameworks and binaries
// clang++ -Wall -Wextra -std=c++11 -F/Library/Frameworks -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer xxx.cpp


#include <string>
#include "game.h"

Game::Game()
{
  isDone = false;
  // default fps in case it's not set later
  fps = 60;
}


Game::~Game()
{
  
}


bool Game::initSystem(int width, int height, bool fullscreen)
{
  // string used for error reporting
  std::ostringstream errorMsg;

  //?? should be done in each file by SDL_INIT_SUBSYSTEMS -- the reason it's
  // done like this is: SDL_Init() initializes assertions and crash protection
  // and then calls SDL_InitSubSystem(). If you want to bypass those protections
  // you can call SDL_InitSubSystem() directly.
  //?? also, I think I initialized the video somewhere else in engine files
  if( SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      errorMsg << "Failed to initialze SDL! SDL Error: " << SDL_GetError();
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL",
			       errorMsg.str().c_str(), NULL);
      return false;
    }

 //  //?? still not completely sure this is needed the documentation is not great
 //  // initialize SDL_image for PNG loading
 //  int imgFlags = IMG_INIT_PNG;
 //  if( !(IMG_Init(imgFlags) & imgFlags) )
 //    {
 //      printf("SDL_Image could not initialize! SDL_Image Error: %s\n",
 //  	     IMG_GetError());
 //    }

  // initialize SDL, create a window and an associated renderer
  if (!graphics.init(width, height, fullscreen))
    {
      return false;
    }

  // initialize SDL_ttf
  // TODO should consider doing this in outlinefont since that's the only thing that requires ttf
  // although this require a check to TTF_WasInit() in order to not initiallize every time font is created
  if( TTF_Init() == -1 )
    {
      printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n"
             , TTF_GetError()); 
      return false;
    }  
  
  
  // TODO: this could be implemented in the future but not sure it's really that necessary
  //  SDL_SetWindowTitle(graphics->getWindow(), title.c_str());

  // initializing input does not return a boolean value
  input.init();

  // initialize gamepad
  if (!gamepad.init())
    {
      SDL_Log("Failed to initailize gamepad.");
      return false;
    }
  
  // initialize sound
  if (!audio.init())
    {
      return false;
    }

  return true;	
}


// cleanup to quit
void Game::freeSystem()
{
  graphics.free();
  input.kill();
  gamepad.kill();
  audio.kill();
  TTF_Quit();
  SDL_Quit();
}


// set the frames per second of the game
void Game::setFPS(int f)
{
  fps = f;
}


// get the time elapsed for the frame
unsigned int Game::getTicks()
{
  //?? this is probably better done with a timer class implementation
  // or by using SDLs built in capabilities
  return SDL_GetTicks();
}


// delay running game by some time
void Game::delay(int ticks)
{
  if (ticks > 0)
    {
      SDL_Delay(ticks);
    }
}


// runs the game
void Game::run()
{
   while (!isDone)
    {
      // investigate SDL's timer functionality
      unsigned int frameStart = SDL_GetTicks();

      SDL_Event event;

      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    {
	      isDone = true;
	      break;
	    }
          // TODO find out if there is some way do this directly within gamepad class
          // or possibly add input and gamepad together and keep in main event loop
          // needed to facilitate the adding or removing of game controllers

          

          
          gamepad.receiveEvent(event);
          // used to facilitate the entering of text into a prompt for instance
          if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
            {
              input.receiveEvent(event);
            }
	}

      input.update();
      gamepad.update();
      // clear here??
      graphics.clear();
      update();
      draw(&graphics);
      graphics.flip();

      int frameTime = getTicks() - frameStart;
      int delayTime = (1000 / fps) - frameTime;

      delay(delayTime);
    }
   
  free();
  freeSystem();
}

// these are the virtual functions that must be overridden
bool Game::init()
{
  // just the default if nothing supplied 
  return (initSystem(800, 600, false));
}


void Game::draw(Graphics* gfx)
{
  // just the default again, if draw not overridden
  gfx->clear();
}

 
void Game::free()
{
}

void Game::update()
{
}
