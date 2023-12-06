// factory.cpp
//
//

#include "factory.h"


bool Factory::init(Map* map, Graphics* gfx)
{
  m_map = map;

  if (!m_soldierImage.load(gfx, "graphics/soldier.bmp", 23, 20))
    return false;

  if (!m_slugImage.load(gfx, "graphics/slug.bmp", 33, 20))
    return false;

  if (!m_riderImage.load(gfx, "graphics/rider.bmp", 50, 40))
    return false;

  if (!m_flierImage.load(gfx, "graphics/flier.bmp", 62, 35))
    return false;

  return true;
}


void Factory::free()
{
  m_soldierImage.free();
  m_slugImage.free();
  m_riderImage.free();
  m_flierImage.free();
}


SceneNode* Factory::create(int entity, int x, int y)
{
  switch (entity)
    {
    case ENTITY_SOLDIER:
      return new Walker(&m_soldierImage, x, y, m_map);
      break;

    case ENTITY_SLUG:
      return new Walker(&m_slugImage, x, y, m_map);
      break;
      
    case ENTITY_RIDER:
      return new Walker(&m_riderImage, x, y, m_map);
      break;

    case ENTITY_FLIER:
      return new Flier(&m_flierImage, x, y, m_map);

    default:
      return new SceneNode(1, x,  y, 50, 50);
    }
}
