//
// optionState.h
//

#ifndef OPTION_STATE_H_
#define OPTION_STATE_H_

#include "core/outlineFont.h"
#include "core/parallax.h"
#include "core/slider.h"
#include "core/image.h"
#include "core/graphics.h"
#include "core/input.h"
#include "core/gamepad.h"
#include "core/gameState.h"
#include "SDL2/SDL_mixer.h"
#include "HUD.h"

class OptionState : public GameState
{
private:

  HUD* p_optionsHUD;
  Parallax* p_background;
  Input* p_input;
  Gamepad* p_gamepad;
  bool hasFocus;
  bool initialized = false;
  
public:
  
  bool init(HUD* optionsHUD, Parallax* background, Input* input, Gamepad* gamepad);
  void update();
  void draw(Graphics* gfx);
  void free();
  bool wasInit() { return initialized; }
};
#endif
