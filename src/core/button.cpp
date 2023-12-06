//
// button.cpp
//
// class to define a button such as for a menu or HUD
#include "button.h"
#include "image.h"
#include <cstdio>


Button::Button()
{
  m_isPressed = false;
  m_isInFocus = false;
  m_isMouseOver = false;
  m_image = NULL;
  m_mouseOverImage = NULL;
  m_pressedImage = NULL;
}

// TODO may be beneficial to implement buttons as a collection/grid --that way only one
// button will be active at a time and the result of a click will only return an enum
bool Button::init(Graphics* gfx, std::string imagePath, std::string overImagePath
		  , std::string pressedImagePath, int width, int height)
{
  // load any images for buttons that were passed in
  if (!imagePath.empty())
    {
      // TODO consider if heap memory is preferable 
      m_image = new Image();
      if (!m_image->load(gfx, imagePath))
	return false;
    }

  if (!pressedImagePath.empty())
    {
      m_pressedImage = new Image();
      if (!m_pressedImage->load(gfx, pressedImagePath))
	return false;
    }

  if (!overImagePath.empty())
    {
      m_mouseOverImage = new Image();
      if (!m_mouseOverImage->load(gfx, overImagePath))
	return false;
    }

  // if width and height not passed (default=-1) then set to the image dimensions
  if (width == -1 || height == -1)
    {
      setWidth(m_image->getWidth());
      setHeight(m_image->getHeight());
    }
  else
    {   // allow for resizing the button if defaults not passed
      setWidth(width);
      setHeight(height);
      
      if (m_image != NULL)
 	{
	  m_image->setDimensions(width, height);
	}
      if (m_mouseOverImage != NULL)
	{
	  m_mouseOverImage->setDimensions(width, height);
	}
      if (m_pressedImage != NULL)
	{
	  m_pressedImage->setDimensions(width, height);
	}
    }

  return true;
}


// allow user to create on the fly buttons from passed font and text
bool Button::createFromString(Graphics* gfx, OutlineFont* font, std::string text, Uint8 r, Uint8 g
                              , Uint8 b, char buttonState, Uint32 renderFlag, SDL_Color bgColor)
{
  Image* img_ptr;

  // TODO make buttons states enums!!
  // TODO take images out of heap maybe
  switch (buttonState)
    {
    case 's':
    case 'S':
      m_image = new Image();
      img_ptr = m_image;
      break;
    case 'o':
    case 'O':
      m_mouseOverImage = new Image();
      img_ptr = m_mouseOverImage;
      break;
    case 'p':
    case 'P':
      m_pressedImage = new Image();
      img_ptr = m_pressedImage;
      break;
      //TODO: throw an error as default:
    }

  //?? should I set m_image to Null here to make sure bellow test works
  font->getFontImage(gfx, img_ptr, text, r, g, b, renderFlag, bgColor);

  if (img_ptr == NULL)
    {
      //TODO create error
      return false;
    }

  // set image dimensions 
  setWidth(img_ptr->getWidth());
  setHeight(img_ptr->getHeight());

  img_ptr = NULL;
  
  return true;
}


void Button::press()
{
  m_isPressed = true;
}


bool Button::isPressed()
{
  if (m_isPressed)
    {
      // here we reset the button to make sure it's state remains until observed
      m_isPressed = false;
      return true;
    }
  else
    {
      return false;
    }
}

void Button::setFocus(bool focusValue)
{
  m_isInFocus = focusValue;
}


void Button::update(Input* input)
{
   //TODO implement a more robust system that responds to gamepad and mouse simultaneously
   // but probably better to decouple mouse from buttons
  m_isMouseOver = false;

  if (isInsideBoundary(input->getMouseX(), input->getMouseY()))
  {
    m_isMouseOver = true;
      
    if (input->mouseDown(Input::MOUSE_LEFT))
    {
      m_isPressed = true;
    }
  }
  else if (m_isInFocus)
  {
    m_isMouseOver = true;
  }
}


  void Button::draw(Graphics* gfx)
  {
     // if mouse is hovering on image, draw mouseoverimage
    if (m_mouseOverImage != NULL && m_isMouseOver)
    {
      m_mouseOverImage->draw(gfx, getX(), getY());
       // draw a border around the button for highlighting / depth
       //gfx->drawRect(getX(), getY(), getWidth(), getHeight(), 45, 50, 60);
       //gfx->drawRect(getX() +1, getY() +1, getWidth() -2, getHeight() -2, 45, 50, 60);
    }
     // if a pressed image is loaded draw it when button pressed
    else if (m_pressedImage != NULL && m_isPressed) 
    {
      m_pressedImage->draw(gfx, getX(), getY());
       // draw a border around the button for highlighting / depth
      gfx->drawRect(getX(), getY(), getWidth(), getHeight(), 45, 50, 60);
      gfx->drawRect(getX() +1, getY() +1, getWidth() -2, getHeight() -2, 45, 50, 60);
    }
    else// just draw the standard button
    {
      m_image->draw(gfx, getX(), getY());
    }

  }


  void Button::free()
  {
     // get rid of all images used in buttons
    if (m_image != NULL)
    {
      m_image->free();
      m_image = NULL;
    }
    if (m_pressedImage != NULL)
    {
      m_pressedImage->free();
      m_pressedImage = NULL;
    }
    if (m_mouseOverImage != NULL)
    {
      m_mouseOverImage->free();
      m_mouseOverImage = NULL;
    }
  }
  
