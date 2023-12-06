// map.h
//
// class to work with tileset to make tilemaps

#ifndef MAP_H_
#define MAP_H_

#include "graphics.h"
#include "image.h"
#include <fstream>
#include <cstdlib> //?? not sure how necessary
#include <cstring> //?? not sure how necessary
#include <vector>


using namespace std;

class Map
{
private:
  Image tileSet;
  int tileWidth;
  int tileHeight;
  // the size of the level measured in tile-counts
  int mapWidth;
  int mapHeight;
  // pointers to arrays containing all the tiles in the map
  int* layer1, *layer2, *layer3, *solidLayer;
  vector<int> solidTiles;

  void loadDimensions(ifstream* in); 
  void loadSolidTiles(ifstream* in);
  void loadLayer(ifstream* in, int* layer);
  void generateSolidLayer();

public:
  Map();
  ~Map();

  bool load(const char mapName[], const char imageName[], Graphics* g);
  //?? should maybe call xOffset xCam and yCam
  void draw(int layer, int xCam, int yCam, Graphics* g);
  void free();
  bool isTileSolid(int tile);
  bool checkSolid(int cellX, int cellY);
  // getter methods
  int getWidth() { return (tileWidth * mapWidth); }
  int getHeight() { return (tileHeight * mapHeight); }
  int getTileWidth() { return tileWidth; }
  int getTileHeight() { return tileHeight; }
  int getNumTilesWide() { return mapWidth; }
  int getNumTilesHigh() { return mapHeight; }
};

#endif
