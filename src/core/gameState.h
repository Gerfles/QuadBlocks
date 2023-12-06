//
// gamestate.h
//
// overwrite this function to add different game states to the game
// manager's stack ie- class SplashState
#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "stateManager.h"
#include "graphics.h"

class StateManager;

class GameState
{
private:
  StateManager* manager;
public:
  GameState() { manager = NULL; }
  ~GameState() {};

  //??? not sure why the below function is needed
  void setManager(StateManager* SM) { manager = SM; }
  void free();
  virtual void update();
  virtual void draw(Graphics* gfx);
  virtual void reset() {};
  //  virtual void free();

  // getter function
  StateManager* getManager() { return manager; }
};
#endif  

