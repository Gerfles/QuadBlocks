//
// mainMenuState.h
//
// splash screen for the start of the game
#ifndef MAIN_MENU_STATE_H_
#define MAIN_MENU_STATE_H_

// core dependencies
#include "core/gameState.h"
#include "core/graphics.h"
#include "core/image.h"
#include "core/input.h"
#include "core/gamepad.h"
#include "core/game.h"
#include "core/music.h"
#include "core/outlineFont.h"
#include "core/button.h"
#include "core/sounds.h"
// project includes
#include "core/parallax.h"
#include "runState.h"
#include "creditsState.h"
#include "optionState.h"
#include "assetList.h"

class MainMenuState : public GameState
{
 private:
  const static int BUTTON_FONT_SIZE = 50;
  const static int BUTTON_X = 300;
  const static int BUTTON_Y = 200;
  const static int BUTTON_SPACING = 65;

  // stats
  int m_screenWidth;
  int m_screenHeight;
  // resources
  Parallax background;
  OutlineFont m_font;
   Image m_title;
  Music m_music;
  HUD optionsHUD;
  // Game States
  CreditsState m_creditState;
  RunState m_runState;
  OptionState m_optionState;
  // buttons (0 = start, 1 = credits, 2 = options, 3 = exit)
  Button buttons[4];
  int activeButton;
  std::string buttonTitles[4] = {"Start", "Credits", "Options", "Exit"};
  // Button m_startButton;
  // Button m_creditsButton;
  // Button m_exitButton;
  Sounds m_sounds;
  // game pointers (awarness)
  Game* p_game;
  Graphics* p_graphics;
  Audio* p_audio;
  Input* p_input;
  Gamepad* p_gamepad;

public: //?? should consider putting destructor calling free()
  MainMenuState() {};
  ~MainMenuState() {};
  // TODO remove Game* since doesnt seem used
  bool init(Graphics* graphics, Input* input, Audio* audio, Gamepad* gamepad);
  void free();
  void update();
  //?? do we need? since we already have graphics pointer?
  // I don't think so but would have to change virtual function properly
  void draw(Graphics* graphics);


  // DEBUG
  //  int exitCode;
  
};
#endif
