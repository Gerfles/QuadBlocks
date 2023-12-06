//
// pieces.cpp
//
#include "pieces.h"
#include "SDL2/SDL_config.h"
#include "SDL2/SDL_keycode.h"
#include <clocale>
#include <cstdio>
#include <iostream>


Piece::Piece(Grid* boundingGrid) : p_boundingGrid(boundingGrid)
{
  
}


bool Piece::rotate(int rotateDirection)
{
  // first rotate
  Grid::rotate(1);
  int previousCol = m_firstCol;

  // then check for collisions and rotate the piece back if any are encountered
  while (collisionColumn() != NO_COLLISION_FLAG)
    {
      // since collision is evaluated true if the piece is outside of boundaries and a rotate
      // can cause that so if the piece is outside boundaries it gets pushed back in
      if (collisionColumn() < 0)
        {
          m_firstCol++;
        }
      else if (collisionColumn() >= p_boundingGrid->getNumCols())
        {
          m_firstCol--;
        }
      else
        {
          // put back--piece still would have collided even after sliding it back into playfield 
          Grid::rotate(-1);
          m_firstCol = previousCol;
          return false;
        }
    }
  return true;
}

bool Piece::rotateBack()
{
  return rotate(BACK_ROTATE);
}

bool Piece::moveLeft()
{
  // first move left
  m_firstCol--;

  // then check for collisions and move the piece back if any are encountered
  if (collisionColumn() != NO_COLLISION_FLAG)
    {
      m_firstCol++;
      return false;
    }

  return true;
}


bool Piece::moveRight()
{
  // first move right
  m_firstCol++;

  // then check for collisions and move the piece back if any are encountered
  if (collisionColumn() != NO_COLLISION_FLAG)
    {
      m_firstCol--;
      return false;
    }
  
  return true;
} 


bool Piece::moveDown()
{
  if (hasLanded())
    {
      return false;
    }
  
  m_topRow++;
  return true;
}


// handle piece instantly dropped event
void Piece::drop()
{
  // continue moving down until colision
  while (!hasLanded())
    {
      moveDown();
    }
}


//TODO represent 0 with empty block and -1 with no collision
// return: grid column where collision occurred or -1 for no collision   
int Piece::collisionColumn()
{
  // search row by row and column by column for an element that has collided
  //with a bounding grid or block inside it
  for (int row = 0; row < getNumRows(); row++)
    { 
      for (int col = 0; col < getNumCols(); col++)
        {
          // make sure to only check the solid blocks of the piece 
          if (getTile(row, col) > 0)
            {
              // define row and column WRT the bounding grid
              int gridRow = row + m_topRow;
              int gridCol = col + m_firstCol;

              // make sure the block is within the bounding Grid
              if (gridCol >= 0 && gridCol < p_boundingGrid->getNumCols()
                  && gridRow < p_boundingGrid->getNumRows())
                {
                  //next check that block hasn't colided with another block
                  if (gridRow >= 0 && p_boundingGrid->getTile(gridRow, gridCol) != 0)
                    {
                      return gridCol;
                    }
                }
              else
                {
                  return gridCol;
                }
            }
        }
    }
  
  // if no overlapping occurred, return not collided (false)
  return NO_COLLISION_FLAG;
}


// return the number of lines cleared but just set the tiles to white for now
int Piece::clearLines()
{
  int clearedLines = 0;

  for (int row = m_topRow; row < getNumRows() + m_topRow; row++)
    {
        for (int col = 0; col < p_boundingGrid->getNumCols(); col++)
        {
          // if any block in this row is empty (0) skip to next row
          if (p_boundingGrid->getTile(row, col) == 0)
            {
              break;
            }
          else // check if we made it to last col, meaning this is a complete line
            {
              if (col == p_boundingGrid->getNumCols() -1)
                {
                  clearedLines++;
                  // set all blocks in this row to special effect tile
                  p_boundingGrid->setRowTiles(row, SPECIAL_EFFECT_TILE);

                }
            }
        }
    }
  return clearedLines;
}

// remove all the special effect tiles and slide down remaining rows
void Piece::removeLines()
{
  // look along all the rows for empty pieces 
  for (int row = 0; row < p_boundingGrid->getNumRows(); row++)
    {
      // assume that if first tile in row is special effect tile the whole row is
      if (p_boundingGrid->getTile(row, 0) == SPECIAL_EFFECT_TILE)
        {
          // set all the blocks to empty
          p_boundingGrid->setRowTiles(row, 0);
          // move the above pieces down
          for (int swapRow = row; swapRow > 0; swapRow--)
            {
              p_boundingGrid->swapRows(swapRow, swapRow - 1);
            }
        }
    }
}

void Piece::draw(Graphics* gfx)
{
  // first set the relative piece position
  setX(p_boundingGrid->getX() + m_firstCol * m_blockDims);
  setY(p_boundingGrid->getY() + m_topRow * m_blockDims);

  // then call the parent class draw
  Grid::draw(gfx);
}


void Piece::draw(Graphics* gfx, int angle)
{
  // first set the relative piece position
  setX(p_boundingGrid->getX() + m_firstCol * m_blockDims);
  setY(p_boundingGrid->getY() + m_topRow * m_blockDims);

  // then call the parent class draw
  Grid::draw(gfx, (double)angle);
}


//TODO think about making these functions inlined 
void Piece::draw(Graphics *gfx, int x, int y)
{
    Grid::draw(gfx, x, y);
}



void Piece::setRow(int row)
{
  m_topRow = row;
}

int Piece::getRow()
{
  return m_topRow;
}

void Piece::setColumn(int column)
{
  m_firstCol = column;
}

int Piece::getCol()
{
  return m_firstCol;
}

bool Piece::hasLanded()
{
  // first move piece down 
  m_topRow++;

  // then check for collisions - meaning the piece is already on top of floor or another piece
  if (collisionColumn() != NO_COLLISION_FLAG)
    {
      // don't forget to move piece back no matter which result we get
      m_topRow--;
      return true;       
    }
  
  m_topRow--;
  return false;
}


L_piece::L_piece(Image* tileSet, Grid* boundingGrid, int blockDims) : Piece(boundingGrid)
{
  
  // initialize Grid and block dimensions
  init(3, 3, blockDims, blockDims, tileSet);
  m_blockDims = blockDims;
  
  // set initial level to determine block color
  m_color = 4;

  // set offset for drawing pipeline pieces
  xOffset = -5;
  yOffset = 12;
  
  // set the block back to beginning rotation and position
  reset();
}

void L_piece::reset()
{
  m_topRow = -2;
  m_firstCol = 3;

  // abstract representation - could also use 3x3
  // [0 1 0]
  // [0 1 0]
  // [0 1 1]
  // first set all elements to 0
  setAllTiles(0);
  
  setTile(0, 1, m_color);
  setTile(1, 1, m_color);
  setTile(2, 1, m_color);
  setTile(2, 2, m_color);
}


S_piece::S_piece(Image* tileSet, Grid* boundingGrid, int blockDims) : Piece(boundingGrid)
{
  // initialize Grid
 init(3, 3, blockDims, blockDims, tileSet);
 m_blockDims = blockDims;
   
  // set initial level to determine block color
  m_color = 2;

  // set offset for drawing pipeline pieces
  xOffset = 5;
  yOffset = 1;

  // set the block back to beginning rotation and position
  reset();
}

void S_piece::reset()
{
  m_topRow = -2;
  m_firstCol = 4;
  // abstract representation - could also use 3x3
  // [0 0 0]
  // [0 1 1]
  // [1 1 0] 
 // first set all elements to 0
  setAllTiles(0);
  
  setTile(1, 1, m_color);
  setTile(1, 2, m_color);
  setTile(2, 0, m_color);
  setTile(2, 1, m_color);
}


Z_piece::Z_piece(Image* tileSet, Grid* boundingGrid, int blockDims) : Piece(boundingGrid)
{
    // initialize Grid
  init(3, 3, blockDims, blockDims, tileSet);
  m_blockDims = blockDims;
    
  // set initial level to determine block color
  m_color = 1;

  // set offset for drawing pipeline pieces
  xOffset = 5;
  yOffset = 1;

  // set the block back to beginning rotation and position
  reset();
}

void Z_piece::reset()
{
  m_topRow = -2;
  m_firstCol = 4;
  
  // abstract representation - could also use 3x3
  // [0 0 0]
  // [1 1 0]
  // [0 1 1] 
  // first set all elements to 0
  setAllTiles(0);
  
  setTile(1, 0, m_color);
  setTile(1, 1, m_color);
  setTile(2, 1, m_color);
  setTile(2, 2, m_color);
}


Line_piece::Line_piece(Image* tileSet, Grid* boundingGrid, int blockDims) : Piece(boundingGrid)
{
  // initialize Grid
  init(4, 4, blockDims, blockDims, tileSet);
  m_blockDims = blockDims;
  
  // set initial level to determine block color
  m_color = 6;

  // set offset for drawing pipeline pieces
  xOffset = 6;
  yOffset = 0;
  
  reset();
}

void Line_piece::reset()
{
  m_topRow = -3;
  m_firstCol = 4;
  
  // abstract representation - could also use 3x3
  // [0 1 0 0]
  // [0 1 0 0]
  // [0 1 0 0] 
  // [0 1 0 0]
  
 // first set all elements to 0
  setAllTiles(0);
  
  setTile(0, 1, m_color);
  setTile(1, 1, m_color);
  setTile(2, 1, m_color);
  setTile(3, 1, m_color);
}


Square_piece::Square_piece(Image* tileSet, Grid* boundingGrid, int blockDims)
  : Piece(boundingGrid)
{
  // initialize Grid
  init(2, 2, blockDims, blockDims, tileSet);
  m_blockDims = blockDims;
    
  // set initial level to determine block color
  m_color = 5;

  // set offset for drawing pipeline pieces
  xOffset = 17;
  yOffset = 25;
  
  reset();
}

void Square_piece::reset()
{
  m_topRow = -1;
  m_firstCol = 4;
  
  // abstract representation - could also use 3x3
  // [1 1]
  // [1 1]
  
 // first set all elements to 0
  setAllTiles(0);
  
  setTile(0, 0, m_color);
  setTile(0, 1, m_color);
  setTile(1, 0, m_color);
  setTile(1, 1, m_color);
}


T_piece::T_piece(Image* tileSet, Grid* boundingGrid, int blockDims) : Piece(boundingGrid)
{
    // initialize Grid
  init(3, 3, blockDims, blockDims, tileSet);
  m_blockDims = blockDims;
    
  // set initial level to determine block color
  m_color = 7;

  // set offset for drawing pipeline pieces
  xOffset = 1;
  yOffset = 12;
  
  reset();
}

void T_piece::reset()
{
  m_topRow = -2;
  m_firstCol = 3;
  
  // abstract representation - could also use 3x3
  // [0 1 0]
  // [0 1 1]
  // [0 1 0] 

  // first set all elements to 0
  setAllTiles(0);

  setTile(0, 1, m_color);
  setTile(1, 1, m_color);
  setTile(1, 2, m_color);
  setTile(2, 1, m_color);
}


BackL_piece::BackL_piece(Image* tileSet, Grid* boundingGrid, int blockDims)
  : Piece(boundingGrid)
{
  // initialize Grid
  init(3, 3, blockDims, blockDims, tileSet);
  m_blockDims = blockDims;
  
  // set initial level to determine block color
  m_color = 3;

  // set offset for drawing pipeline pieces
  xOffset = 18;
  yOffset = 12;
  
  reset();
}

void BackL_piece::reset()
{
  m_topRow = -2;
  m_firstCol = 4;

  // abstract representation - could also use 3x3
  // [0 1 0]
  // [0 1 0] 
  // [1 1 0]

 // first set all elements to 0
  setAllTiles(0);  

  setTile(0, 1, m_color);
  setTile(1, 1, m_color);
  setTile(2, 0, m_color);
  setTile(2, 1, m_color);
}

L_ghost::L_ghost(Image* tileSet, Grid* boundingGrid, int blockDims) : L_piece(tileSet, boundingGrid, blockDims)
{

}

void L_ghost::reset()
{
  m_topRow = -2;
  m_firstCol = 3;
  // here the m_color refers to the row of the appropriate color sprites
  m_color = 4; 
  // abstract representation - could also use 3x3
  // [0 1 0]
  // [0 1 0]
  // [0 1 1]
  // first set all elements to 0
  setAllTiles(0);

  // Orange 
  setTile(0, 1, m_color * 19 + 3);
  setTile(1, 1, m_color * 19 + 17);
  setTile(2, 1, m_color * 19 + 8);
  setTile(2, 2, m_color * 19 + 2);
}



void S_ghost::reset()
{
  m_topRow = -2;
  m_firstCol = 4;
  // here the m_color refers to the row of the appropriate color sprites
  m_color = 2; 
  // abstract representation - could also use 3x3
  // [0 0 0]
  // [0 1 1]
  // [1 1 0] 
 // first set all elements to 0
  setAllTiles(0);
  
  setTile(1, 1, m_color * 19 + 5);
  setTile(1, 2, m_color * 19 + 2);
  setTile(2, 0, m_color * 19 + 0);
  setTile(2, 1, m_color * 19 + 10);
}

void Z_ghost::reset()
{
  m_topRow = -2;
  m_firstCol = 4;
  // here the m_color refers to the row of the appropriate color sprites
  m_color = 1;   
  // abstract representation - could also use 3x3
  // [0 0 0]
  // [1 1 0]
  // [0 1 1] 
  // first set all elements to 0
  setAllTiles(0);
  
  setTile(1, 0, m_color * 19 + 0);
  setTile(1, 1, m_color * 19 + 7);
  setTile(2, 1, m_color * 19 + 8);
  setTile(2, 2, m_color * 19 + 2);
}

void Line_ghost::reset()
{
  m_topRow = -3;
  m_firstCol = 4;
  // here the m_color refers to the row of the appropriate color sprites
  m_color = 6; 
  // abstract representation - could also use 3x3
  // [0 1 0 0]
  // [0 1 0 0]
  // [0 1 0 0] 
  // [0 1 0 0]
  
 // first set all elements to 0
  setAllTiles(0);
  
  setTile(0, 1, m_color * 19 + 3);
  setTile(1, 1, m_color * 19 + 17);
  setTile(2, 1, m_color * 19 + 17);
  setTile(3, 1, m_color * 19 + 4);
}

void Square_ghost::reset()
{
  m_topRow = -1;
  m_firstCol = 4;
  
  // abstract representation - could also use 3x3
  // [1 1]
  // [1 1]
  
 // first set all elements to 0
  setAllTiles(0);
  
  setTile(0, 0, m_color * 19 + 11);
  setTile(0, 1, m_color * 19 + 12);
  setTile(1, 0, m_color * 19 + 13);
  setTile(1, 1, m_color * 19 + 14);
}

void T_ghost::reset()
{
  m_topRow = -2;
  m_firstCol = 3;
  // here the m_color refers to the row of the appropriate color sprites
  m_color = 7; 
  // abstract representation - could also use 3x3
  // [0 1 0]
  // [0 1 1]
  // [0 1 0] 

  // first set all elements to 0
  setAllTiles(0);

  setTile(0, 1, m_color * 19 + 3);
  setTile(1, 1, m_color * 19 + 15);
  setTile(1, 2, m_color * 19 + 2);
  setTile(2, 1, m_color * 19 + 4);
}

void BackL_ghost::reset()
{
  m_topRow = -2;
  m_firstCol = 4;
  // here the m_color refers to the row of the appropriate color sprites
  m_color = 3; 
  // abstract representation - could also use 3x3
  // [0 1 0]
  // [0 1 0] 
  // [1 1 0]

 // first set all elements to 0
  setAllTiles(0);  

  setTile(0, 1, m_color * 19 + 3);
  setTile(1, 1, m_color * 19 + 17);
  setTile(2, 0, m_color * 19 + 0);
  setTile(2, 1, m_color * 19 + 10);
}

