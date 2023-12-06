//
// HUD.cpp
//
// Heads up display
 #include "HUD.h"
// #include "SDL2/SDL_scancode.h"

bool HUD::init(Graphics *graphics, Input *input, Gamepad *gamepad)
{
  p_graphics = graphics;
  p_input = input;
  p_gamepad = gamepad;
  currSoundVolume = 10;
  currMusicVolume = 10;
  highlightedSlider = 0;


  if (!UIframe.load(graphics, "graphics/futureui2.png"))
    return false;

  // load the label fonts
  if (!soundLabel.load("text/ss_font.ttf", 38) || !musicLabel.load("text/ss_font.ttf", 38))
    return false;

  musicLabel.createFontImage(p_graphics, "Music Volume", 96, 128, 230);
  soundLabel.createFontImage(p_graphics, "Sound Volume", 96, 128, 230);
  
  // initialize the music volume slider
  if (!musicVolume.init(graphics, gamepad, input, "graphics/sliderbody.png"
                        , "graphics/sliderknob.png", "graphics/sliderbar.png"
                        , "graphics/sliderhighlight.png", "", 30, 0, 10))
    {
      return false;
    }

  // initialize the sound volume slider
  if (!soundVolume.init(graphics, gamepad, input, "graphics/sliderbody.png"
                        , "graphics/sliderknob.png", "graphics/sliderbar.png"
                        , "graphics/sliderhighlight.png", "", 30, 0, 10))
    {
      return false;
    }

  // initialize the volume slider as the active slider
  musicVolume.setHighlight(true);
  soundVolume.setHighlight(false);
  musicVolume.activate();
  soundVolume.deactivate();

  return true;
  
} // END init()


bool HUD::isActive()
{
  return (musicVolume.isActivated() || soundVolume.isActivated());
}


void HUD::update()
{
  // only update if a new button is hit on the gamepad
  if (p_gamepad->anyButtonHit())
    {
  
      if (p_gamepad->isButtonHit(DPAD_UP))
        {
          highlightedSlider--;

          // scroll loop-around
          if (highlightedSlider < 0)
            highlightedSlider = 1;
        }

      if (p_gamepad->isButtonHit(DPAD_DOWN))
        {
          highlightedSlider++;

          // scroll loop-around
          if (highlightedSlider > 1)
            highlightedSlider = 0;
        }

      // music volume slider is highlighted
      if (highlightedSlider == 0)
        {
          musicVolume.setHighlight(true);
          soundVolume.setHighlight(false);
          musicVolume.activate();
          soundVolume.deactivate();
        }
      else
        {
          soundVolume.setHighlight(true);
          musicVolume.setHighlight(false);
          soundVolume.activate();
          musicVolume.deactivate();
        }
    }
}

void HUD::draw()
{
  UIframe.draw(p_graphics, 102, 60);

  musicLabel.draw(p_graphics, 386, 190);
  soundLabel.draw(p_graphics, 386, 350);

  // update and draw the current slider bars 
  int newMusicVolume = musicVolume.draw(p_graphics, 356, 250);
  int newSoundVolume = soundVolume.draw(p_graphics, 356, 410);

    // check to see if we need to adjust the volume
  if (newMusicVolume != currMusicVolume)
    { // set the volume of the music
      Mix_VolumeMusic(newMusicVolume);
      currMusicVolume = newMusicVolume;
    }

  if (newSoundVolume != currSoundVolume)
    {
      Mix_Volume(-1, newSoundVolume);
      currSoundVolume = newSoundVolume;
    }
}


void HUD::free()
{
  musicVolume.free();
  soundVolume.free();

  soundLabel.free();
  musicLabel.free();
  UIframe.free();
}



