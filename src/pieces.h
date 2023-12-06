//
// pieces.h
//
#ifndef PIECES_H_
#define PIECES_H_

#include "core/grid.h"

// define all the pieces used in tetris
class Piece : public Grid
{
private:
  // the background grid that represents the playfield, or pipeline, etc.
  Grid* p_boundingGrid;
  // ?? const static vs const
  static const int NO_COLLISION_FLAG = -99;

  static const int SPECIAL_EFFECT_TILE = 9;
  //  static const int CLEAR_FLAG = 7;
protected:
  // determines which block frame to use
  int m_color;
  // representing the top left element position in relationship to the bounding grid
  int m_topRow;
  int m_firstCol;
  int m_blockDims; // size of both width and height
  int xOffset;
  int yOffset;

public:
  static const int FORWARD_ROTATE = 1;
  static const int BACK_ROTATE = -1;
  //TODO use a single image for all pieces without using a pointer (static?)
  Piece(Grid* boundingGrid);
  // note that making the base class destructor virtual makes the derived class destructors
  // virtual as well (in this case default, compiler generated destructors)
  virtual ~Piece() {};
  virtual void reset() = 0;
  bool rotate(int rotateDirection = 1);
  bool rotateBack();
  bool moveLeft();
  bool moveRight();
  bool moveDown();
  void drop();
  void setColumn(int collumn);
  void setRow(int row);
  
  // return the collumn a collision occurred with or NO_COLLISION_FLAG if false
  int collisionColumn();
  // remove completed lines
  int clearLines();
  void removeLines();
  void draw(Graphics* gfx);
  // overload to draw pieces in any position (not associated with bounding grid)
  void draw(Graphics* gfx, int x, int y);
  // also overload to draw pieces that can rotate the tiles for drawing ghost pieces
  void draw(Graphics* gfx, int angle);

  // getters
  int getRow();
  int getCol();
  int getYOffset() {return yOffset;} 
  int getXOffset() {return xOffset;}
  bool hasLanded();
};

 
class L_piece : public Piece
{
public:
  L_piece(Image* tileSet, Grid* boundingGrid, int blockDims);
  virtual void reset();
};

class S_piece : public Piece
{
public:
  S_piece(Image* tileSet, Grid* boundingGrid, int blockDims);
  virtual void reset();
};

class Z_piece : public Piece
{
public:
  Z_piece(Image* tileSet, Grid* boundingGrid, int blockDims);
  virtual void reset();
};

class Line_piece : public Piece
{
public:
  Line_piece(Image* tileSet, Grid* boundingGrid, int blockDims);
  virtual void reset();
};

class Square_piece : public Piece
{
public:
  Square_piece(Image* tileSet, Grid* boundingGrid, int blockDims);
  virtual void reset();
};

class T_piece : public Piece
{
public:
  T_piece(Image* tileSet, Grid* boundingGrid, int blockDims);
  virtual void reset();
};

class BackL_piece : public Piece
{
public:
  BackL_piece(Image* tileSet, Grid* boundingGrid, int blockDims);
  virtual void reset();
};

// Ghost pieces 
class L_ghost : public L_piece
{
public:
  L_ghost(Image* tileSet, Grid* boundingGrid, int blockDims); 
    
  virtual void reset();
};

class S_ghost : public S_piece
{
public:
  S_ghost(Image* tileSet, Grid* boundingGrid, int blockDims)
    : S_piece(tileSet, boundingGrid, blockDims) {}
  
  virtual void reset();
};

class Z_ghost : public Z_piece
{
public:
  Z_ghost(Image* tileSet, Grid* boundingGrid, int blockDims)
  : Z_piece(tileSet, boundingGrid, blockDims) {}
  virtual void reset();
};

class Line_ghost : public Line_piece
{
public:
  Line_ghost(Image* tileSet, Grid* boundingGrid, int blockDims)
    : Line_piece(tileSet, boundingGrid, blockDims) {}
  virtual void reset();
};

class Square_ghost : public Square_piece
{
public:
  Square_ghost(Image* tileSet, Grid* boundingGrid, int blockDims)
    : Square_piece(tileSet, boundingGrid, blockDims) {}
  virtual void reset();
};

class T_ghost : public T_piece
{
public:
  T_ghost(Image* tileSet, Grid* boundingGrid, int blockDims)
    : T_piece(tileSet, boundingGrid, blockDims) {}
  virtual void reset();
};

class BackL_ghost : public BackL_piece
{
public:
  BackL_ghost(Image* tileSet, Grid* boundingGrid, int blockDims)
    : BackL_piece(tileSet, boundingGrid, blockDims) {}        
  virtual void reset();
};
#endif
