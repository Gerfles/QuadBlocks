//
// textPrompt.h
//
// Class to draw a text prompt to screen
#ifndef TEXT_PROMPT_H
#define TEXT_PROMPT_H

#include <string>
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_stdinc.h"
#include "input.h"
#include "outlineFont.h"
#include "graphics.h"
#include "gamepad.h"

class TextPrompt
{
private:  
  std::string m_text;
  Graphics* p_graphics;
  Input* p_input;
  Gamepad* p_gamepad;
  OutlineFont m_font;
  OutlineFont m_promptChar;
  Timer blinkTimer;
  char activeChar; 
  int m_r, m_g, m_b; // colors to render text
  Uint32 m_renderFlag; // to set type of outlinefont render
  Uint32 m_alignment;
  bool m_renderText; // to make sure we only render text that has been changed in some way

  
public:
  enum alignment {CENTER, LEFT, RIGHT};
  bool load(Graphics* gfx, Input* input, Gamepad* gamepad, std::string fontFile, int fontSize,
            int red = 255, int green = 255, int blue = 255, Uint32 alignment = CENTER);
  // return true is text prompt is open and false if prompt has been closed
  bool draw(int x, int y);
  std::string getText();
  void enableTextInput();
  void setRenderType(Uint32 renderFlag) { m_renderFlag = renderFlag; }
  void free();
};
#endif
