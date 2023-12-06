//
// mapNode.h
//
#ifndef MAP_NODE_H_
#define MAP_NODE_H_

#include "map.h"
#include "sceneNode.h"


class MapNode : public SceneNode
{
private:
  Map* m_map;
  bool m_isSolid;
public:
  MapNode(int id, int x, int y, int width, int height, bool solid, Map* m);
  ~MapNode() {};

  void move(int x, int y);
  bool overlapsMap();

  virtual void draw(Graphics* gfx, Rectangle* camera);
};
#endif


