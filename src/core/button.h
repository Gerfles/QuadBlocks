//
// button.h
//
// class to define a button such as for a menu or HUD
#ifndef BUTTON_H_
#define BUTTON_H_

#include "image.h"
#include "input.h"
#include "gamepad.h"
#include "rectangle.h"
#include "outlineFont.h"


//using std::string;

class Button : public Rectangle
{
private:
  // the standard image for the button
  Image* m_image;
  // the image when mouse is over button
  Image* m_mouseOverImage;
  // the button pressed image
  Image* m_pressedImage;
  bool m_isPressed;
  bool m_isMouseOver;
  bool m_isInFocus;
public:
  Button();
  ~Button() {}; //free(); };
  bool init(Graphics* gfx, std::string imagePath, std::string overImagePath = ""
	    , std::string pressedImagePath = "", int width = -1, int height = -1);
  // allows the creation of the button image from a text string
  bool createFromString(Graphics* gfx, OutlineFont* font, std::string text
			,Uint8 r, Uint8 g, Uint8 b, char buttonState = 's'
			,Uint32 renderFlag = OutlineFont::SHADED
			,SDL_Color bgColor = {0x00,0x00,0x00,0xFF});
  
  void update(Input* input);
  void draw(Graphics* gfx);
  void setFocus(bool focusValue);
  void press();
  bool isPressed();
  void free();
};
#endif
