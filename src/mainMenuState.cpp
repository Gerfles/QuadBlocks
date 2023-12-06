// mainMenustate.cpp
//
//

#include "mainMenuState.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_scancode.h"



bool MainMenuState::init(Graphics* graphics, Input* input, Audio* audio
			 , Gamepad* gamepad)
{
  p_graphics = graphics;
  p_input = input;
  p_audio = audio;
  p_gamepad = gamepad;
  
  activeButton = 0;
  
  // set the resolution for the app 
  m_screenWidth = graphics->getScreenWidth();
  m_screenHeight = graphics->getScreenHeight();

  // add the parallaxing background
    std::vector<std::string> fileNames = {"graphics/layer1.png", "graphics/layer2.png", "graphics/layer3.png"
    , "graphics/layer4Lights.png", "graphics/layer4.png", "graphics/layer5.png", "graphics/layer6.png"
    , "graphics/layer7Lights.png", "graphics/layer7.png", "graphics/layer8.png", "graphics/layer9.png"};

  if (!background.load(graphics, fileNames, 0, 13, Parallax::RIGHT))
    return false;

  // load the music here to be used throughout the game
  fileNames.clear();
  fileNames = {"audio/arcade-music-loop.ogg", "audio/enchanted_forest_loop.ogg"
    , "audio/happy-8bit-pixel-adventure.ogg", "audio/ockaie_temple.ogg", "audio/sci_fi_platformer.ogg"};

  if (!m_music.load(fileNames))
      return false;

  // load the sound effects here to be used throughout the game  
  //TODO test if need clear or not;
  fileNames.clear();
  fileNames = {"audio/boop.wav", "audio/coin.wav", "audio/line_clear.wav", "audio/select.wav"
    , "audio/splat.ogg", "audio/star.ogg", "audio/tada.ogg", "audio/click.ogg"};

  if (!m_sounds.load(fileNames))
    return false;

   // load the title image
  m_title.load(graphics, "graphics/title.png");


   // associate the loaded music and sound with audio so it can be used in every game state without reloading
  p_audio->setMusic(&m_music);
  p_audio->setSounds(&m_sounds);

  // play the main menu song
  //TODO create a slow looping function that fades out and waits x time to begin again
  p_audio->music->playTrack(0, -1);

  // TODO add hook to music finished -- insanely difficult if not impossible!!!
  //  Mix_HookMusicFinished(&m_music.playnext);
  
  // set the volume of sounds to something reasonable (initial volume is 128 and not linear scale)
  Mix_Volume(-1, 10);
  // set the volume of the music
  Mix_VolumeMusic(10);

  // load the font for the buttons
  if (!m_font.load("text/ss_font.ttf", BUTTON_FONT_SIZE))
      return false;
  
  // create buttons for main menu
  for (int i = 0; i < 4; i++)
    {
      // create standard-state of the buttons on the main menu screen 
      if (!buttons[i].createFromString(p_graphics, &m_font, buttonTitles[i], 10, 100, 10
                                       , 's', OutlineFont::BLENDED))
      {
        return false;
      }
      
      // set active-button colors
      buttons[i].createFromString(p_graphics, &m_font, buttonTitles[i], 80, 180, 80,
                                  'o', OutlineFont::BLENDED);
 
      // place the buttons on the screen
      buttons[i].setPos(BUTTON_X + i * 30, BUTTON_Y +  i * BUTTON_SPACING);
    }

  // initialize the Heads up display used for optionsState and for the pause menu
  optionsHUD.init(graphics, input, gamepad);

  return true;
}


void MainMenuState::free()
{
  // end any gamestates that haven't been already closed
  if (m_runState.wasInit())
    {
      m_runState.free();
    }

  if (m_creditState.wasInit())
    {
      m_creditState.free();
    }

  if (m_optionState.wasInit())
    {
      m_optionState.free();
    }

  optionsHUD.free();

  // free all resources 
  p_audio->stopMusic();
  m_music.free();
  background.free();
  m_font.free();
  m_music.free();
  m_sounds.free();
  m_title.free();
  
  // free all buttons
  for (int i = 0; i < 4; i++)
    {
      buttons[i].free();
    }
}


void MainMenuState::update()
{
  //make sure music is playing the right track
  //  m_music.update();

  // update each button
  for (int i = 0; i < 4; i++)
    {
      buttons[i].update(p_input);
    }

  // scroll down
  if (p_gamepad->isButtonHit(DPAD_DOWN) || p_input->keyHit(SDL_SCANCODE_DOWN))
    {
      buttons[activeButton].setFocus(false);
      activeButton++;

      p_audio->sounds->play(BUTTON_CHANGE_SOUND);
      
      if (activeButton > 3)
        activeButton = 0;
    }

  // scroll up
  if (p_gamepad->isButtonHit(DPAD_UP) || p_input->keyHit(SDL_SCANCODE_UP))
    {
      buttons[activeButton].setFocus(false);
      activeButton--;

      p_audio->sounds->play(BUTTON_CHANGE_SOUND);
      
      if(activeButton < 0)
        activeButton = 3;
    }

   // activate the newly selected button
  buttons[activeButton].setFocus(true);      

  // select button
  if (p_gamepad->isButtonHit(A_BUTTON) || p_gamepad->isButtonHit(START_BUTTON)
      || p_input->keyHit(SDL_SCANCODE_RETURN))
    {
      buttons[activeButton].press();
      p_audio->sounds->play(BUTTON_SELECT_SOUND);
    }

  
  // START button
  if (buttons[0].isPressed()) //|| p_input->keyHit(SDL_SCANCODE_RETURN))
    {
      // load the runstate if need be and add it to the gamestate stack
      if (!m_runState.wasInit())
        {
          m_runState.init(p_graphics, p_input, p_gamepad, p_audio, m_screenWidth
                          , m_screenHeight, &optionsHUD);
        }
      m_runState.reset();
      getManager()->addState(&m_runState);
    }
  // CREDITS button
  else if (buttons[1].isPressed())
    {
      // load the creditstate if need be and add it to the gamestate stack
      if (!m_creditState.wasInit())
        {
          m_creditState.init(p_graphics, p_input, p_gamepad, p_audio);
        }
      getManager()->addState(&m_creditState);
    }
  // OPTIONS button
  else if (buttons[2].isPressed())
    {
      // load the option state if need be and add it to the gamestate stack
      if (!m_optionState.wasInit())
        {
          m_optionState.init(&optionsHUD, &background, p_input, p_gamepad);
        }
      //add the option state to the gamestate stack
      getManager()->addState(&m_optionState);
    }
  // EXIT button
  else if (buttons[3].isPressed() || p_input->keyHit(SDL_SCANCODE_ESCAPE))
    {
      // completely empty the stack so our game knows to quit
      getManager()->popState();
    }
}


void MainMenuState::draw(Graphics* graphics)
{
  // draw the background and title to the screen
  background.draw(graphics, 0, 0);

  m_title.draw(graphics, 150, 60);

  for (int i = 0; i < 4; i++)
    {
      buttons[i].draw(graphics);
    }
}

