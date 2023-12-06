// factory.h
//
//

#ifndef FACTORY_H_
#define FACTORY_H_

#include "image.h"
#include "map.h"
#include "sceneNode.h"
#include "flier.h"
#include "walker.h"

enum{ENTITY_SLUG, ENTITY_SOLDIER, ENTITY_RIDER, ENTITY_FLIER};

class Factory
{
private:
  Map* m_map;
  Image m_soldierImage;
  Image m_slugImage;
  Image m_riderImage;
  Image m_flierImage;
public:
  bool init(Map* map, Graphics* gfx);
  void free();
  SceneNode* create(int entity, int x, int y);
};

#endif
