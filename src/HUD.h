//
// HUD.h
//
// Heads Up Display (options)


#ifndef HUD_H_
#define HUD_H_

#include "core/outlineFont.h"
#include "core/slider.h"
#include "core/image.h"
#include "core/graphics.h"
#include "core/input.h"
#include "core/gamepad.h"
#include "core/gameState.h"
#include "SDL2/SDL_mixer.h"

class HUD
{
private:

  Slider musicVolume;
  Slider soundVolume;
  int currMusicVolume;
  int currSoundVolume;
  Image UIframe;
  OutlineFont musicLabel;
  OutlineFont soundLabel;
  int highlightedSlider;

  // shared resources
  Input* p_input;
  Gamepad* p_gamepad;
  Graphics* p_graphics;
  
public:
  
  bool init(Graphics *graphics, Input *input, Gamepad *gamepad);
  void update();
  void draw();
  void free();
  bool isActive();
};
#endif


