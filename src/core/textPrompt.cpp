// textPrompt.cpp
//
//
// Class to draw a text prompt to screen

#include "textPrompt.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "outlineFont.h"
#include <iostream>

bool TextPrompt::load(Graphics* gfx, Input* input, Gamepad* gamepad, std::string fontFile
                      , int fontSize, int red, int green, int blue, Uint32 alignment)
{
  // copy over parameters
  p_graphics = gfx;
  p_input = input;
  p_gamepad = gamepad;
  m_alignment = alignment;
  m_r = red;
  m_g = green;
  m_b = blue;
    
  // open the font for rendering text 
  if (!m_font.load(fontFile, fontSize))
      return false;

  // create the blinking underscore char to signify that text need be input
  if (!m_promptChar.load(fontFile, fontSize))
    return false;

  // render the blinking line to signify prompt
  m_promptChar.createFontImage(p_graphics, "_", m_r, m_g, m_b, OutlineFont::BLENDED);
  // set the active char to one before A
  activeChar = 64; 
  return true;
  
} // END LOAD()


// TODO consider combining this with into draw
void TextPrompt::enableTextInput()
{
  p_input->enableTextInput(&m_text);

  // start the timer for the blinking prompt
  blinkTimer.start();
  
} // END ENABLETEXTINPUT()


bool TextPrompt::draw(int x, int y)
{
  // indicator of whether to redraw text -1 = quit, 1 = redraw, 0 = no change
    int textInputStatus = p_input->textInputStatus();

    // exit the text input if user hits start button
    if (p_gamepad->isButtonHit(START_BUTTON))
      {
        SDL_StopTextInput();
        textInputStatus = -1;
      }

    // allow gamepad to cycle through letters (a->z)
    if (p_gamepad->isButtonHit(DPAD_UP))
      {
      activeChar++;
      
      if (activeChar > 90)
        {
          activeChar = 64;
          m_promptChar.createFontImage(p_graphics, " ", m_r, m_g, m_b, OutlineFont::BLENDED);
        }
      else
        {
          m_promptChar.createFontImage(p_graphics, std::string(1, activeChar), m_r, m_g, m_b, OutlineFont::BLENDED);
        }
    }

    // allow gamepad to cycle down through letters (z->a)
    if (p_gamepad->isButtonHit(DPAD_DOWN))
    {
      activeChar--;
      
      if (activeChar < 65)
        {
          activeChar = 91;
          m_promptChar.createFontImage(p_graphics, " ", m_r, m_g, m_b, OutlineFont::BLENDED);
        }
      else
        {
          m_promptChar.createFontImage(p_graphics, std::string(1, activeChar), m_r, m_g, m_b, OutlineFont::BLENDED);
        }
    }

    // allow a button to set the char to the end of our string
  if (p_gamepad->isButtonHit(A_BUTTON))
    {
      if (activeChar > 64)
        {
          m_text += activeChar;
          textInputStatus = 1;
        }
      else
        {
          m_text += " ";
          textInputStatus = 1;
        }
    }

  // allow b button to erase a character from our string
    if (p_gamepad->isButtonHit(B_BUTTON))
    {
      m_text.pop_back();
      textInputStatus = 1;
    }
  
  if (textInputStatus == -1)
    { // text prompt no longer open for input
      return false;
    }
  else if (textInputStatus == 1)
    {
      // make sure we don't try to render an empty string (SDL_TTF doesn't like that)
      if (m_text.empty())
        {
          m_font.createFontImage(p_graphics, " ", m_r, m_g, m_b, OutlineFont::BLENDED);
        }
      else
        { // draw new text string
          m_font.createFontImage(p_graphics, m_text, m_r, m_g, m_b, OutlineFont::BLENDED);
        }
    }



  // let xPos represent where to draw the text in relation to x, depending on alignment
  int xPos;
  
  switch (m_alignment)
    {
    case CENTER:
      { // center text to the width of the 
        xPos = x - m_font.getWidth() / 2;
        break;
      }
    case LEFT:
      {
        xPos = x;
        break;
      }
    case RIGHT:
      {
        xPos = x - m_font.getWidth();
        break;
      }
    default:
      break;
    }

  // draw font to the screen and given alignment and position
  m_font.draw(p_graphics, xPos, y);

  
  // draw the blinking underscore to signify prompt for text flashing every half second
  if (blinkTimer.getTicks() > 200 && blinkTimer.getTicks() < 800)
    {
        m_promptChar.draw(p_graphics, xPos + m_font.getWidth(), y);
    }
  else  if (blinkTimer.getTicks() > 800)
    {
      // reset timer to zero
      blinkTimer.start();
    }
  
  return true;
  
} // END DRAW()


std::string TextPrompt::getText()
{
  return m_text;
} // END GETTEXT()


void TextPrompt::free()
{
  p_graphics = NULL;
  p_input = NULL;
  m_font.free();
  m_promptChar.free();
} // END FREE()
