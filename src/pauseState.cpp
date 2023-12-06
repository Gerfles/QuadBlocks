//
// pauseState.cpp
//
//TODO implement statistics display

#include "pauseState.h"
#include "parallax.h"
#include "runState.h"

bool PauseState::init(Input* in, Gamepad* gamepad, StateManager* manager
                      , RunState* prevState, HUD* optionsHUD)
{
  // grab keyboard input and set this state to respond to our state manager
  p_input = in;
  setManager(manager);
  p_prevState = prevState;
  p_gamepad = gamepad;
  p_optionsHUD = optionsHUD;
  //TODO draw instructions to screen

  return true;
}


void PauseState::free()
{
}


void PauseState::update()
{
  // check if we need to exit pause state
  if (p_input->keyHit(SDL_SCANCODE_RETURN) || p_input->keyHit(SDL_SCANCODE_ESCAPE)
      || p_gamepad->isButtonHit(START_BUTTON))
    {  
      p_prevState->unpauseTimer();
      getManager()->popState();
    }

  // allow to exit game from pause screen
  if (p_input->keyHit(SDL_SCANCODE_Q) || p_gamepad->isButtonHit(BACK_BUTTON))
    {
      getManager()->popState();
      getManager()->popState();
    }

  p_optionsHUD->update();
}

void PauseState::draw(Graphics* gfx)
{
  // draw only the background and console and stats--no blocks
  p_prevState->drawEnvironment();

  // draw the slider
  p_optionsHUD->draw();
}
