#ifndef FLIER_H_
#define FLIER_H_

#include "mapNode.h"
#include "sprite.h"

class Flier : public MapNode
{
private:
  int xVel;
  Map* map;
  Sprite sprite;
public:
  Flier(Image* i, int x, int y, Map* m);
  ~Flier(){};

  void update();
  void draw(Graphics* g, Rectangle* view);
};

#endif
