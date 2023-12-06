// creditsState.h
//
// state to give credit for game

#ifndef CREDITSSTATE_H_
#define CREDITSSTATE_H_

#include "core/gameState.h"
#include "core/graphics.h"
#include "core/image.h"
#include "gamepad.h"
//#include "core/music.h"
#include "audio.h"
#include "core/outlineFont.h"
#include "core/button.h"
#include "core/parallax.h"
#include "core/textOverlay.h"

class CreditsState : public GameState
{
private:
  
  // class resources
  Parallax background;
  TextOverlay credits;
  
  OutlineFont m_font;
  Button m_mainMenuButton;
  //Music m_song;
  bool initialized = false;
  // shared resources
  Graphics* p_graphics;
  Audio* p_audio;
  Input* p_input;
  Gamepad* p_gamepad;

public:
  CreditsState();
  bool init(Graphics* graphics, Input* input, Gamepad* gamepad, Audio* audio);
  void free();
  void update();
  void draw(Graphics* graphics);

  bool wasInit() {return initialized;}
  
};

#endif

