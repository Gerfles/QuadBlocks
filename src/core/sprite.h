// sprite.h
//
//

#ifndef SPRITE_H_
#define SPRITE_H_


#include "image.h"
//?? not sure why this was included
//#include "rectangle.h"

class Sprite
{
private:
  Image* m_image;
  int m_firstFrame;
  int m_lastFrame;
  int m_currentFrame;
  int m_delay;
  int m_delayCounter;
public:
  Sprite();
  ~Sprite() {};
  void setImage(Image* image) { m_image = image; }
  void setAnimation(int first, int last, int delay);
  void setFrame(int frame);
  //void setDimensions(int width, int height);
  //void resize(int percent);
  void update();
  void draw(Graphics* gfx, int x, int y, int w = -1, int h = -1);
  // getter methods
  Image* getImage() { return m_image; }
  int getWidth();
  int getHeight();
};

#endif
