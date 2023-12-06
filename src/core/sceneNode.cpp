// sceneNode.cpp
//
//


#include "sceneNode.h"

SceneNode::SceneNode(int i, int ix, int iy, int iwidth, int iheight) : Rectangle(ix, iy, iwidth, iheight)
{
  ID = i;
  removed = false;
  scene = NULL;
}


void SceneNode::draw(Graphics* gfx, Rectangle* camera)
{
  // makes sure the sceneNode is in the viewport and if so, draws red rectangle around it
  //?? not sure why we need to regularly check if graphics is null
  if (camera->overlaps(*this) && gfx != NULL)
    {
      // draw a red rectangle around the sceneNode
      gfx->drawRect(getX() - camera->getX(), getY() - camera->getY()
		    , getWidth(), getHeight(), 255, 0, 0, 255);
    }
}
