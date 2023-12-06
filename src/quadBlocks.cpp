// tetris.cpp
//
//

#include "quadBlocks.h"
#include "core/stateManager.h"


bool Tetris::init()
{
  setFPS(60);
  
  // seed random number generator
  srand(time(NULL));

  if (!initSystem(SCREEN_WIDTH, SCREEN_HEIGHT, false))
    {
      return false;
    }
  
  if (!mainMenuState.init(getGraphics(), getInput(), getAudio(), getGamepad()))
    {
      return false;
    }

 // add just the menu state to the stack and load substates from there
  stateManager.addState(&mainMenuState);

  return true;
}


void Tetris::free()
{
  mainMenuState.free();
}


void Tetris::update()
{
  if (stateManager.isEmpty())
    {
      end();
      return;
    }
  
  stateManager.update();
}


void Tetris::draw(Graphics* gfx)
{
  stateManager.draw(gfx);
}

