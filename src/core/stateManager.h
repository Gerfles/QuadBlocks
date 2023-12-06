// stateManager.cpp
//
//

#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#include <stack>
#include "gameState.h"
#include "graphics.h"

class GameState;

class StateManager
{
private:
  std::stack<GameState*> states;
public:
  StateManager(){};
  ~StateManager(){};

  void addState(GameState* state);
  GameState* peekState();
  void popState();
  void update();
  void draw(Graphics* gfx);
  bool isEmpty();
};

#endif




