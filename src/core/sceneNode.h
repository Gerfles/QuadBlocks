// sceneNode.h
//
//

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include "graphics.h"
#include "scene.h"
#include "rectangle.h"

class Scene;

class SceneNode : public Rectangle
{
private:
  Scene* scene;
  bool removed;
  int ID;
public:
  //?? don't think default constructor needed
  SceneNode(){};
  SceneNode(int i, int x, int y, int width, int height);
  //?? note that we have made the destructor virtual in order to bypass a certain
  // error but I'm not sure this is the best method to deal with new and delete of // sceneNodes
  virtual ~SceneNode() {};
  // scene management
  void setScene(Scene* s) { scene = s; };
  virtual void update() {};
  virtual void draw(Graphics* gfx, Rectangle* view);
  void remove() { removed = true; };
  bool isRemoved() { return removed; } 
  // getter methods
  Scene* getScene() { return scene; }
  int getID() { return ID; };
};

#endif
