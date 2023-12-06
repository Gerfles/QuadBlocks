// map.cpp
//
// class to work with tileset to make tilemaps

#include "map.h"


void Map::loadDimensions(ifstream* in)
{
  std::string buffer;
  
  // get rid of the word 'width'
  getline(*in, buffer, ' ');
  // get the following data and
  getline(*in, buffer, '\n');
  // convert that char to an integer
  mapWidth = atoi(buffer.c_str());

  // do the same as above for the 'height' data
  getline(*in, buffer, ' ');
  getline(*in, buffer, '\n');
  mapHeight = atoi(buffer.c_str());

  // do the same as above for the 'tile width' data
  getline(*in, buffer, ' ');
  getline(*in, buffer, '\n');
  tileWidth = atoi(buffer.c_str());

  // do the same as above for the 'tile height' data
  getline(*in, buffer, ' ');
  getline(*in, buffer, '\n');
  tileHeight = atoi(buffer.c_str());
}


void Map::loadSolidTiles(ifstream* in)
{
  std::string buffer;

  //Get the line that says 'solid_tiles' and ignore it
  getline(*in, buffer, '\n');
  
  //Get the row containing solid tiles and save it to a c_string
  //?? could implement using boost libraries-- such as boost tokenizer
  // https://www.boost.org/doc/libs/1_72_0/libs/tokenizer/doc/char_separator.htm
  getline(*in, buffer, '\n');
  
  char* string = new char[buffer.size() + 1];
  strcpy(string, buffer.c_str());
  
  // find the first occurence of the token to delimit based on
  char* token = strtok(string, " ,");
  
  while(token != NULL)
    {
      // add number to solid tiles vector
      solidTiles.push_back(atoi(token));
      // find next occurence of delimeter
      token = strtok(NULL, " ,");
    }

  // clean up and get rid of dynamically allocated char array
  delete[] string;
}


void Map::loadLayer(ifstream* in, int* layer)
{
  std::string buffer;

  // get the row contains 'layerX' and ignore it
  getline(*in, buffer, '\n');

  for (int y = 0, i = 0; y < mapHeight; y++)
    {
      // reset delimeter to comma 
      char delim = ',';

      for(int x = 0; x < mapWidth; x++)
	{
	  // change the delimeter to \n when on the last element
	  if (x == mapWidth - 1)
	    {
	      delim = '\n';
	    }

	  // look for numbers only and convert them to int and save in array
	  getline(*in, buffer, delim);
	  layer[i] = atoi(buffer.c_str());
	  i++;
	}
    }
}


void Map::generateSolidLayer()
{
  // go through each layer and if there is a solid tile in that spot, indicate it with 1
  for (int i = 0; i < mapWidth * mapHeight; i++)
    {
      if (isTileSolid(layer1[i]) || isTileSolid(layer2[i]) || isTileSolid(layer3[i]))
	{
	  solidLayer[i] = 1;
	}
      else
	{
	  solidLayer[i] = 0;
	}
    }
}


Map::Map()
{
  tileWidth = -1;
  tileHeight = -1;
  mapWidth = -1;
  mapHeight = -1;
  layer1 = layer2 = layer3 = solidLayer = NULL;
}


Map::~Map()
{
  
}


bool Map::load(const char mapName[], const char imageName[], Graphics* g)
{

  layer1 = layer2 = layer3 = solidLayer = NULL;

  ifstream in(mapName);

  if(!in.good())
    {
      printf("Unable to open file: %s\n", mapName);
      return false;
    }

  std::string buffer;

  // get the map dimensions
  loadDimensions(&in);

  // save the tiles that'll be considered solid
  loadSolidTiles(&in);

  // create space to save the tile data
  layer1 = new int[mapWidth * mapHeight];
  layer2 = new int[mapWidth * mapHeight];
  layer3 = new int[mapWidth * mapHeight];
  solidLayer = new int[mapWidth * mapHeight];

  // load all layers into stack space
  loadLayer(&in, layer1);
  loadLayer(&in, layer2);
  loadLayer(&in, layer3);
  
  // create the solid layer
  generateSolidLayer();

  in.close();

  // load tileset image
  if(!tileSet.load(g, imageName, tileWidth, tileHeight))
    {
      printf("Failed to load tile set image!\n");
      return false;
    }

  return true;
}


void Map::draw(int layer, int xCam, int yCam, Graphics* g)
{
  int* drawLayer = NULL;

  // set the layer that we want to draw
  switch (layer)
    {
    case 0:
      drawLayer = solidLayer;
      break;
    case 1:
      drawLayer = layer1;
      break;
    case 2:
      drawLayer = layer2;
      break;
    case 3:
      drawLayer = layer3;
      break;
    default:
      return;
    }

  // calculate where to start drawing the scene from using integer division so that
  // if 99% of the tile is off screen, still draw that tile
  int startY = yCam/tileHeight;
  int startX = xCam/tileWidth;

  // calculate how many rows and columns will be on the screen
  //?? would maybe be good to make this a static number or at least avoid calculating
  // each call to draw (I think the +2 is there to pad top/bottom & left/right) such
  // that if the camera is halfway between two tiles, it draws them both
  //?? seems it might only need +1 -- try with that value
  int rows = (g->getScreenHeight() / tileHeight) + 2;
  int columns = (g->getScreenWidth() / tileWidth) + 2;

  // cover each row of the (tile-sized ) grid from left to right
  for (int y = startY; y < startY + rows; y++)
    {
      // cover each column of the (tile-sized ) grid from left to right
      for (int x = startX; x < startX + columns; x++)
	{
	  // set frame to -1 each iteration to make sure we don't draw tiles off the level
	  int frame = -1;

	  // determine whether the cell is inside the level and if so calculate
	  // the frame of the spritesheet to use 
	  if (x >= 0 && y >= 0 && x < mapWidth && y < mapHeight)
	    {
	      // find the array entry that corresponds to the tile we're rendering
	      frame = drawLayer[y * mapWidth + x] - 1;
	    }

	  // only draw the cells that were on the screen
	  if (frame >= 0)
	    {
	      //draw a red rectangle around solid objects (using the solid layer) 
	      if (layer == 0)
		{
		  g->drawRect(x * tileWidth - xCam, y * tileHeight - yCam
			      , tileWidth, tileHeight, 255, 0, 0, 255);
		}
	      else
		{ // draw the tile onto the screen
		  tileSet.drawFrame(g, x * tileWidth - xCam, y * tileHeight - yCam
			     , frame);
		}
	    }
	}
    }
}


void Map::free()
{
  // check each layer and delete if need be
  if (layer1 != NULL)
    {
      delete[] layer1;
      layer1 = NULL;
    }
  
  if (layer2 != NULL)
    {
      delete[] layer2;
      layer2 = NULL;
    }
    
  if (layer3 != NULL)
    {
      delete[] layer3;
      layer3 = NULL;
    }
      
  if (solidLayer != NULL)
    {
      delete[] solidLayer;
      solidLayer = NULL;
    }

  solidTiles.clear();
  tileSet.free();
}


bool Map::isTileSolid(int tile)
{
  // loop though our vector of solid tiles and see if the pased tile is in it
  for (int knownSolid : solidTiles)
    {
      if (tile == knownSolid)
	return true;
    }
  
  return false;
}


// examine solid layer of map to see if the tile at the x and y location is
// solid or not. Return true if solid and false otherwise. 
bool Map::checkSolid(int cellX, int cellY)
{
	
  // check that tile location is within the map and if not, return false
  if (cellX < 0 || cellX > mapWidth || cellY < 0 || cellY > mapHeight)
    {
      return false;
    }

  return solidLayer[cellY * mapWidth + cellX];
}


