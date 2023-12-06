//
// mapNode.cpp
//
#include "mapNode.h"

MapNode::MapNode(int id, int x, int y, int width, int height, bool solid, Map* m)
  : SceneNode(id, x, y, width, height)
{
  m_map = m;
  m_isSolid = solid;
}


void MapNode::draw(Graphics* gfx, Rectangle* camera)
{
      gfx->fillRect(getX() - camera->getX(), getY() - camera->getY()
		    , getWidth(), getHeight(), 0, 255, 0, 255);
}

//?? this solution seem inelegant should look for more intuitive soln
bool MapNode::overlapsMap()
{
  if (m_map == NULL)
    {
      return false;
    }

  // check every point from the upper-left corner of the rectangle to the
  // lower-right corner
  int tileWidth = m_map->getTileWidth();
  int tileHeight = m_map->getTileHeight();
  
  for (int x = getX(); x < getX2(); x += tileWidth)
    {
      for (int y = getY(); y < getY2(); y += tileHeight)
	{
	  if (m_map->checkSolid(x / tileWidth, y / tileHeight))
	    {
	      return true;
	    }
	}
      // check the bottom edge of the rectangle
      if (m_map->checkSolid(x / tileWidth, getY2() / tileWidth))
	{
	  return true;
	}
    }
  // check the right edge of the rectangle
  for (int y = getY(); y < getY2(); y += tileHeight)
    {
      if (m_map->checkSolid(getX2() / tileWidth,  y / tileHeight))
	{
	  return true;
	}
    }

  // check the bottom-left corner of the rectangle
  return (m_map->checkSolid(getX2() / tileWidth, getY2() / tileHeight));
}
 

void MapNode::move(int x, int y)
{
  // if the mapNode (object) is not solid or the map doesn't exist
  // just move the object withot collision detection
  if (!m_isSolid || m_map == NULL)
    {
      setX(getX() + x);
      setY(getY() + y);
      return;
    }

  // first move along the x asis
  setX(getX() + x);

  // check for a collision if the object is moving left
  if (x < 0)
    {
      if (overlapsMap())
	{
	  // react to the collision
	  setX(((getX()/m_map->getTileWidth()) + 1) * m_map->getTileWidth());
	}
    }

  // check for a collision if the object is moving right
  if (x > 0)
    {
      if (overlapsMap())
	{
	  // react to the collision
	  setX2((getX2() / m_map->getTileWidth()) * m_map->getTileWidth() - 1);
	}
    }

  setY(getY() + y);

  //check for a collision if the object is moving up
  if (y < 0)
    {
      if (overlapsMap())
  	{
  	  // react to the collision
  	  setY(((getY() / m_map->getTileHeight()) + 1) * m_map->getTileHeight());
  	}
    }

  // check for a collision if the object is moving down
  if (y > 0)
    {
      if (overlapsMap())
	{
	  setY2((getY2() / m_map->getTileHeight()) * m_map->getTileHeight() - 1);
	}
    }
}
