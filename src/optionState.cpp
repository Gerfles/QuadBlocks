//
// optionState.cpp
//
#include "optionState.h"
#include "SDL2/SDL_scancode.h"
#include "core/parallax.h"


bool OptionState::init(HUD* optionsHUD, Parallax* background, Input *input, Gamepad *gamepad)
{
  p_input = input;
  p_gamepad = gamepad;
  p_background = background;
  p_optionsHUD = optionsHUD;
  initialized = true;
  hasFocus = true;
  
  return true;
  
} // END init()


void OptionState::update()
{
  // check if we want to exit option state
  if (p_gamepad->isButtonHit(BACK_BUTTON) || p_input->keyHit(SDL_SCANCODE_ESCAPE)
      || p_gamepad->isButtonHit(B_BUTTON))// && hasFocus)
    {
      getManager()->popState();
    }

  p_optionsHUD->update();
  //  hasFocus = !p_optionsHUD->isActive();
}


void OptionState::draw(Graphics *gfx)
{
  p_background->draw(gfx, 0, 0);
  p_optionsHUD->draw();
}


void OptionState::free()
{
  // don't free HUD since it's reused by runstate
   initialized = false;
}

