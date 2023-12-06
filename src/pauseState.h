//
// pauseState.h
//
// the paused state of the game -- no pieces drawn
#ifndef PAUSE_STATE_H_
#define PAUSE_STATE_H_

// #include "runState.h"
#include "core/gameState.h"
#include "core/input.h"
#include "core/graphics.h"
#include "core/image.h"
#include "core/parallax.h"
#include "core/textOverlay.h"
#include "core/stateManager.h"
#include "core/gamepad.h"
#include "HUD.h"

// declare a runstate class since this class definition requires the RunState
// and the RunState depends on the definition of a PauseState
class RunState;

class PauseState : public GameState
{
private:
  Input* p_input;
  Gamepad* p_gamepad;
  Parallax* p_background;
  Graphics* p_graphics;
  // the background drawing state and the options (slider) state
  RunState* p_prevState;
  HUD* p_optionsHUD;
  
public:

  bool init(Input* in, Gamepad* gamepad, StateManager* manager,
            RunState* prevState, HUD* optionsHUD);
  
  virtual void update();
  virtual void draw(Graphics* gfx);
  void free();
};
#endif
