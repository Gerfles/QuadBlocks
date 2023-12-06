//
//  slider.cpp
//
#include "slider.h"
#include "input.h"


Slider::Slider()
{
  barOffsetX = 0;
  barOffsetY = 0;
  knobIsActivated = false;
  highlightSlider = false;
}


bool Slider::init(Graphics* gfx, Gamepad* gamepad, Input* input, std::string sliderImgFile
                  , std::string knobImgFile, std::string barImgFile, std::string highlightImgFile
                  , std::string knobHighlightFile, int maximum, int divisions, int knobPosition)
{
  m_maximum = maximum;
  m_divisions = divisions;
  p_gamepad = gamepad;
  p_input = input;
  
  // load the slider image which is the box around everything
  if (!m_sliderImg.load(gfx, sliderImgFile))
    return false;

  // load the knob image which is the button used to drag the slider higher or lower
  if (!m_knobImg.load(gfx, knobImgFile))
    return false;
  
  // highlighting of slider body is optional so if user passes empty string, don't load
  if (!highlightImgFile.empty())
    {
      if (!m_sliderHighlight.load(gfx, highlightImgFile))
        return false;

      highlightOffsetX = (m_sliderImg.getWidth() - m_sliderHighlight.getWidth()) / 2;
      highlightOffsetY = (m_sliderImg.getHeight() - m_sliderHighlight.getHeight()) / 2;
    }
  
  // progress bar is optional so if user passes empty string, don't load
  if (!barImgFile.empty())
    {
      if(!m_barImg.load(gfx, barImgFile))
        return false;

      barOffsetX = (m_sliderImg.getWidth() - m_barImg.getWidth()) / 2;
      barOffsetY = (m_sliderImg.getHeight() - m_barImg.getHeight()) / 2;
    }

  //  knob activated image is optional so if user passes empty string, don't load
  if (!knobHighlightFile.empty())
    {
      if (!m_knobHighlightImg.load(gfx, knobHighlightFile))
        return false;
    }
  
  // initialize the knob X offset to middle of slider if no initial position is passed 
  if (knobPosition == -1)
    {
      knobOffsetX = (m_sliderImg.getWidth() - m_knobImg.getWidth()) / 2 ;
    }
  else
    {
      // 
      knobOffsetX = ((float)knobPosition / m_maximum) * m_barImg.getWidth()
        + barOffsetX - (m_knobImg.getWidth() / 2);
    }

  //
  knobOffsetY = (m_sliderImg.getHeight() - m_knobImg.getHeight()) / 2;
  
  return true;
}


void Slider::update(int xPos, int yPos)
{
  // handle slider moving left or right
  if (p_input->mouseDown(Input::MOUSE_LEFT))
    {
     
      // ?? should maybe include a 10 pixel buffer so the mouse can just be close to the knob
      int mouseX = p_input->getMouseX();
      int mouseY = p_input->getMouseY();
      int knobXPos = xPos + knobOffsetX;
      int knobYPos = yPos + knobOffsetY;

      // check to make sure the mouse is within the boundaries of the knob
      if (mouseX >= knobXPos && mouseY >= knobYPos && mouseX <= knobXPos + m_knobImg.getWidth()
          && mouseY <= knobYPos + m_knobImg.getWidth())
        {
          // activate slider knob separately from moving it so deviating in the y dir.
          // while still holding down the mouse button will slide the knob
          activate();
        }

      // when the knob has been activate, track the horizontal position of the mouse
      if (knobIsActivated)
        {
          knobOffsetX = mouseX - xPos;

          // check left boundary to make sure we keep knob inside bar
          if (knobOffsetX < barOffsetX)
            {
              knobOffsetX = barOffsetX;
            }

          // check right boundary to make sure we keep knob inside bar
          if (knobOffsetX > m_sliderImg.getWidth() - barOffsetX - m_knobImg.getWidth())
            {
              knobOffsetX = m_sliderImg.getWidth() - barOffsetX - m_knobImg.getWidth();
            }
        }
    }
  // handle gamepad input but rely on the gamestate class with the slider to activate via
  // gamepad
  else if (knobIsActivated)
    {
      if (p_gamepad->isButtonPressed(DPAD_LEFT))
        {
          knobOffsetX -= 4;
        }

      if (p_gamepad->isButtonPressed(DPAD_RIGHT))
        {
          knobOffsetX += 4;
        }

      // check left boundary to make sure we keep knob inside bar
      if (knobOffsetX < barOffsetX)
        {
          knobOffsetX = barOffsetX;
        }

      // check right boundary to make sure we keep knob inside bar
      if (knobOffsetX > m_sliderImg.getWidth() - barOffsetX - m_knobImg.getWidth())
        {
          knobOffsetX = m_sliderImg.getWidth() - barOffsetX - m_knobImg.getWidth();
        }
    }

  // deactivate knob if user releases mouse button
  if (p_input->mouseUp(Input::MOUSE_LEFT))
    {
      knobIsActivated = false;
    }
}


int Slider::draw(Graphics* gfx, int xPos, int yPos)
{
  // update the postions of all components 
  update(xPos, yPos);

  // draw highlighted background if necessary 
  if (highlightSlider && m_sliderHighlight.isLoaded())
    {
      m_sliderHighlight.draw(gfx, xPos + highlightOffsetX, yPos + highlightOffsetY);
    }

  m_sliderImg.draw(gfx, xPos, yPos);

  // draw the progress bar only if it was passed in (initialized)
  if (m_barImg.isLoaded())
    {
      // draw the bar but only up to the knob
      SDL_Rect partialBar =
        {0, 0, knobOffsetX - barOffsetX + m_knobImg.getWidth() / 2, m_barImg.getHeight()};
      m_barImg.draw(gfx, xPos + barOffsetX, yPos + barOffsetY, &partialBar);
    }

  // draw knob effects if the knob is currently active
  if (knobIsActivated && m_knobHighlightImg.isLoaded())
    {
      m_knobHighlightImg.draw(gfx, xPos + knobOffsetX, yPos + knobOffsetY);
    }
  
  // draw knob last (on top)
  m_knobImg.draw(gfx, xPos + knobOffsetX, yPos + knobOffsetY);

  // return the current position of the knob in proportion to the slider bar
  return m_maximum * (knobOffsetX - barOffsetX) / (m_barImg.getWidth() - m_knobImg.getWidth());
}


void Slider::free()
{
  m_sliderImg.free();
  m_knobImg.free();
  
  if (m_barImg.isLoaded())
    m_barImg.free();

  if (m_sliderHighlight.isLoaded())
    m_sliderHighlight.free();

  if (m_knobHighlightImg.isLoaded())
    m_knobHighlightImg.free();
}



