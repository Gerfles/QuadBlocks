//
// gameState.cpp
//
#include"gameState.h"

void GameState::update()
{
  
}

// void GameState::reset()
// {
  
// }


void GameState::draw(Graphics* gfx)
{
  // maybe should add default statement to suppress warning
  gfx->clear();
}

void GameState::free()
{
  manager = NULL;
} 
