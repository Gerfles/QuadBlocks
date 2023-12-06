// walker.cpp
//
//

#include "walker.h"

Walker::Walker(Image* i, int x, int y, Map* m) : MapNode(1, x, y, i->getFrameWidth(), i->getFrameHeight(), true, m)
{
  sprite.setImage(i);
  yVel = 0;
  xVel = 5;
  sprite.setAnimation(2, 3, 2);

  oldY = getY();

  map = m;
}

void Walker::update()
{
  if(getY() == oldY) //Not falling
    {
      yVel = 1;
      sprite.update();
      
      //Moving Right
      if(xVel > 0)
        {
	  bool edge = !map->checkSolid((getX2()+1)/map->getTileWidth()
				       , (getY2()+1)/map->getTileHeight());
	  bool wall = map->checkSolid((getX2()+1)/map->getTileWidth()
				      , (getY2())/map->getTileHeight());
	  if(edge || wall)
            {
	      xVel*=-1;
	      sprite.setAnimation(0, 1, 2);
            }
        }
      // moving left
      if(xVel < 0)
        {
	  bool edge = !map->checkSolid((getX()-1)/map->getTileWidth()
				       , (getY2()+1)/map->getTileHeight());
	  bool wall = map->checkSolid((getX()-1)/map->getTileWidth()
				      , (getY2())/map->getTileHeight());

	  if(edge || wall)
            {
	      xVel*=-1;
	      sprite.setAnimation(2, 3, 2);
            }
        }

      oldY = getY();
      move(xVel, yVel);
    }
  else
    {
      yVel++;

      oldY = getY();
      move(0, yVel);
    }

  if(getY() > map->getHeight())
    {
      remove();
    }
}

void Walker::draw(Graphics* g, Rectangle* view)
{
  sprite.draw(g, getX()-view->getX(), getY()-view->getY());
}
