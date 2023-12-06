// scene.h
//
//

#ifndef SCENE_H_
#define SCENE_H_

#include <list>
#include "sceneNode.h"
#include "rectangle.h"

class SceneNode;

class Scene
{
private:
  std::list<SceneNode*> nodes;
public:
  Scene() {};
  ~Scene() {};
  // scene management methods
  void addNode(SceneNode* node);
  void removeNode(SceneNode* node);
  // methods that act upon the SceneNode
  void update();
  void draw(Graphics* g, Rectangle* camera);
  // getter methods
  std::list<SceneNode*>* getNodes() { return &nodes; }
};

#endif
