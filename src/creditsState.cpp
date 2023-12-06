// creditsState.cpp
//
// implement last but add scrolling credits with snarky comments

#include "creditsState.h"

CreditsState::CreditsState()
{
  initialized = false;
}


bool CreditsState::init(Graphics* graphics, Input* input, Gamepad* gamepad, Audio* audio)
{
  p_graphics = graphics;
  p_input = input;
  p_audio = audio;
  p_gamepad = gamepad;

  // load the parallaxing background
  std::vector<std::string> filenames = {"graphics/layer1.png", "graphics/layer2.png", "graphics/layer3.png"
    , "graphics/layer4Lights.png", "graphics/layer4.png", "graphics/layer5.png"
    , "graphics/layer6.png", "graphics/layer7Lights.png", "graphics/layer7.png"
    , "graphics/layer8.png", "graphics/layer9.png"};
  background.load(graphics, filenames, 0, 5, Parallax::RIGHT);

  if (!credits.load(graphics, "text/credits.ovl"))
    {
      return false;
    }

  initialized = true;
  
  return true;
}


void CreditsState::free()
{
  background.free();
  credits.free();
  initialized = false;
}


void CreditsState::update()
{
  if (p_input->keyHit(SDL_SCANCODE_ESCAPE) || p_gamepad->isButtonHit(B_BUTTON))
    {
      credits.reset();
      getManager()->popState();
    }
}


void CreditsState::draw(Graphics* graphics)
{
  background.draw(graphics, 0, 0);
  if (credits.draw() == -1)
    {
      credits.reset();
      getManager()->popState();
    }
}


