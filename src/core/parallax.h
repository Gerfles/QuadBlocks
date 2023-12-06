//
// parallax.h
//
// parallaxing image that automates it's own drawing
#ifndef PARALLAX_H_
#define PARALLAX_H_

//#include "SDL2/SDL_image.h"
#include "graphics.h"
#include "timer.h"
#include <vector>
#include <string>
#include "image.h"


class Parallax
{
private:
  //
  std::vector<Image*> layers;
  int m_speed;
  double m_speedStep;
  int width;
  float* xPosArr;
  int xPos, yPos;
  Timer timer;
  Uint32 prevTime;
  int m_direction;

  
public:
  enum direction {RIGHT = 1, LEFT = -1};
  Parallax();
  ~Parallax();
  bool load(Graphics* gfx, std::vector<std::string> filenames, int speed,
            float speedStep, int direction);
  void draw(Graphics* gfx, int x, int y);
  void free();
};
#endif

