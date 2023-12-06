//
// parallax.cpp
//
#include "parallax.h"
#include "SDL2/SDL_log.h"
#include <string>
#include <vector>

Parallax::Parallax()
{
  xPos = 0;
  yPos = 0;
  prevTime = 0;
}


Parallax::~Parallax()
{}


bool Parallax::load(Graphics* gfx, std::vector<std::string> filenames, int speed
                    , float speedStep, int direction )
{
  m_speed = speed;
  m_speedStep = speedStep;
  m_direction = direction;

  // create an array for the various x positions of the various layers
  xPosArr = new float[filenames.size()];

  // open all of the image layers 
  for (int i = 0; i < filenames.size(); i++)
    {
      Image* temp = new Image();
      // attempt to open image file
      if (!temp->load(gfx, filenames[i]))
        {
          SDL_Log("Parallax Can't load Image: %s", filenames[i].c_str());
          return false;
        }
      // place the image layers in the vector and initialize the xPosition to 0
      layers.push_back(temp);
      xPosArr[i] = 0;
    }

  // set "screen width" to the width of the bottom layer
  width = layers[0]->getWidth();
  
  return true;
}

// TODO allow only the screen to be drawn (so parallax could be a PIP)
void Parallax::draw(Graphics *gfx, int xOffset, int yOffset)
{
  // make sure the timer is running 
  if (!timer.hasStarted())
    timer.start();

  // figure out what our current step size should be
  Uint32 elapsedTime = timer.getTicks() - prevTime;
  prevTime = timer.getTicks();
  
  // draw, moving each layer a different amount
  for (int i = 0; i < layers.size(); i++)
    {
      // calculate the new position of the current layer and round for smoother anim.
      xPosArr[i] -= m_direction * ((m_speedStep * i + m_speed) * elapsedTime / 1000.F);
      
      // reset x to zero if image has completely scrolled off screen
      if (xPosArr[i] < -width || xPosArr[i] > width)
        xPosArr[i] = 0;
          
      // draw the layer as well as a mirror image one screen width away for scrolling
      layers[i]->draw(gfx, xPosArr[i] + 0.5, yPos + yOffset);
      layers[i]->draw(gfx, xPosArr[i] + m_direction * width + 0.5, yPos + yOffset);
    }
  //  SDL_Log("layers[8] xPos: %f", xPosArr[8]);
}


void Parallax::free()
{
  timer.stop();
  
  for (Image* layer : layers)
    {
      delete layer;
    }

  delete[] xPosArr;
}


