// scene.cpp
//
//

#include "scene.h"


void Scene::addNode(SceneNode* node)
{
  node->setScene(this);
  nodes.push_back(node);
}


void Scene::removeNode(SceneNode* node)
{
  if (node != NULL)
    {
      //?? note that we are deleting the node using delete keyword which implies
      // the nodes added to scene must be created using the new keyword. This
      // seems sloppy and perhaps should be done inside of each class.
      SceneNode* n = node;
      nodes.remove(node);
      delete n;

      //?? try the following instead
      // nodes.remove(node);
      // delete node;
    } 
  else
    {
      // if passed NULL, delete all nodes
      while(!nodes.empty())
	{
	  // use recursion to get rid of all nodes 
	  removeNode(*nodes.begin());
	}
    }
}


void Scene::update()
{
  // // first get rid of any nodes that have been flagged for removal
  // for (SceneNode* node : nodes)
  //   {
  //     if (node->isRemoved())
  // 	{
  // 	  //?? I believe the idea in setting this node to a scenenode* so that
  // 	  // its virtual nature is ensured and it will call on the virtual function
  // 	  // I also don't think this is necessary since removeNode does that for us
  // 	  SceneNode* oldNode = node;
  // 	  removeNode(oldNode);
  // 	}
  //   }

  // // update all nodes in the scene
  // for (SceneNode* node : nodes)
  //   {
  //     node->update();
  //   }

  std::list<SceneNode*>::iterator it;

    //Remove 'removed' nodes
  for(it = nodes.begin(); it != nodes.end(); it++)
    {
        if((*it)->isRemoved())
        {
            SceneNode* oldNode = (*it);
            it--;
            removeNode(oldNode);
        }
    }

    //Update Nodes
    for(it = nodes.begin(); it != nodes.end(); it++)
    {
        (*it)->update();
    }
}


void Scene::draw(Graphics* gfx, Rectangle* camera)
{
  // go through and draw each node in the scene
  for (SceneNode* node : nodes)
    {
      node->draw(gfx, camera);
    }
}


