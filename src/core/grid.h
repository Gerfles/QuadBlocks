//
// grid.h
//
// class to create an array-like structure of tiles
#ifndef GRID_H_
#define GRID_H_

#include "graphics.h"
#include "image.h"

//TODO implement map as a grid derived class

class Grid
{
private:
  // pointer to dynamically allocated (contiguous) memory for grid tiles
  int* m_tileMatrix;
  // pointer to image containing spritesheet of tile possibilities
  Image* m_tileSet;
  int m_xPos, m_yPos;
  int m_numRows, m_numCols;
  int m_tileWidth;
  int m_tileHeight; //?? might not need if always square (not sure draw works otherwise)
public:
  Grid() {};
  void init(int numRows, int numCols, int tileWidth, int tileHeight, Image* tileSet);
  ~Grid() {};

  void draw(Graphics* gfx);
  // overload to draw grid at any given location
  void draw(Graphics* gfx, int x, int y);
  // overload to draw grid after rotating the individual tiles by angle
  void draw(Graphics* gfx, double angle);
  void merge(Grid* persistentGrid, int row, int col);
  void swapRows(int row1, int row2);
  void rotate(int direction);

  void free();
  bool isTileSolid(int row, int col);

  // setter methods
  void setX(int x) { m_xPos = x; }
  void setY(int y) { m_yPos = y; }
  void setTile(int row, int col, int tile);
  void setRowTiles(int row, int tile);
  void setColTiles(int col, int tile);
  void setAllTiles(int element);
  
  // getter methods
  int getX() { return m_xPos; }
  int getY() { return m_yPos; }
  int getTile(int row, int col);
  int getWidth() { return (m_tileWidth * m_numCols); }
  int getHeight() { return (m_tileHeight * m_numRows); }
  int getTileWidth() { return m_tileWidth; }
  int getTileHeight() { return m_tileHeight; }
  int getNumRows() { return m_numRows; };
  int getNumCols() { return m_numCols; }

  // debugging methods
  void print();
};
#endif
