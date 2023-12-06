//
// stateManager.cpp
//
#include "stateManager.h"

void StateManager::addState(GameState *state)
{
  state->setManager(this);
  states.push(state);
  // ?? should be implement an initiallization on addstate? ie.
  // state->init();
}


void StateManager::popState()
{
  states.pop();
}


GameState* StateManager::peekState()
{
  return states.top();
}


void StateManager::update()
{
  if (!states.empty())
    {
      if (states.top() != NULL)
	{
	  states.top()->update();
	}
    }
}


void StateManager::draw(Graphics* gfx)
{
  if(!states.empty())
    {
      if (states.top() != NULL)
	{
	  states.top()->draw(gfx);
	}
    }
}


bool StateManager::isEmpty()
{
  return states.empty();
}
