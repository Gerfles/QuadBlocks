// walker.h
//
//

#ifndef WALKER_H_
#define WALKER_H_

#include "mapNode.h"
#include "sprite.h"


class Walker : public MapNode
{
 private:
  int yVel;
  int xVel;
  int oldY;
  Map* map;
  Sprite sprite;
 public:
  Walker(Image* i, int x, int y, Map* m);
  ~Walker(){};

  void update();
  void draw(Graphics* gfx, Rectangle* view);
};


#endif
