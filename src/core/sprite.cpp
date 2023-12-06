// sprite.cpp
//
//

#include "sprite.h"

Sprite::Sprite()
{
  m_image = NULL;
  m_firstFrame = 0;
  m_lastFrame = 0;
  m_currentFrame = 0;
  m_delay = 0;
  m_delayCounter = 0;
}


void Sprite::setAnimation(int first, int last, int delay)
{
  m_firstFrame = first;
  m_currentFrame = first;
  m_lastFrame = last;
  m_delay = delay;
  m_delayCounter = 0;
}


void Sprite::setFrame(int frame)
{
  m_currentFrame = m_firstFrame = m_lastFrame = frame;
  m_delay = 0;
  m_delayCounter = 0;
}


void Sprite::update()
{
  m_delayCounter++;

  if (m_delayCounter > m_delay)
    {
      m_delayCounter = 0;
      m_currentFrame++;
    }

  if (m_currentFrame > m_lastFrame)
    {
      m_currentFrame = m_firstFrame;
    }
}


void Sprite::draw(Graphics* gfx, int x, int y, int w, int h)
{
  if (m_image != NULL)
    {
      m_image->drawFrame(gfx, x, y, m_currentFrame, w, h);
    }
}


int Sprite::getWidth()
{
  if (m_image == NULL)
    {
      return -1;
    }

  return m_image->getFrameWidth(); 
}

int Sprite::getHeight()
{
  if (m_image == NULL)
    {
      return -1;
    }

  return m_image->getFrameHeight();
}

