//
// grid.cap
//
// class to create an array-like structure of tiles
#include "grid.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_timer.h"


void Grid::init(int numRows, int numCols, int tileWidth, int tileHeight, Image* tileSet)
{
  m_numRows = numRows;
  m_numCols = numCols;
  m_tileHeight = tileHeight;
  m_tileWidth = tileWidth;
  m_tileSet = tileSet;
  m_xPos = 0;
  m_yPos = 0;
  
  // allocate grid memory on the free store but using one contiguous region of memory instead of
  // using arrays of arrays. Note the () after array size will (check) initialize all elements to 0
  m_tileMatrix = new int[numRows * numCols];

  // initialize all elements to 0 (used to indicate empty block)
  setAllTiles(0);
}


void Grid::draw(Graphics* gfx)
{
  // keep track of position in array to copy memory directly without call to getTile()
  int arrayIndex = 0;
  
  // iterate through all tiles of the grid and draw
  for (int row = 0; row < m_numRows; row++)
    {
      for (int col = 0; col < m_numCols; col++)
        {
          int frame = m_tileMatrix[arrayIndex];

          // draw only the blocks that are not empty (empty represented by 0)
          // note: this makes the first (0) element of the tile array mostly useless
          if (frame > 0)
            {
              m_tileSet->drawFrame(gfx, m_xPos + col * m_tileWidth, m_yPos + row * m_tileHeight, frame);
            }
          arrayIndex++;
        }
    }
}


void Grid::draw(Graphics* gfx, double angle)
{
  // keep track of position in array to copy memory directly without call to getTile()
  int arrayIndex = 0;
  
  // iterate through all tiles of the grid and draw
  for (int row = 0; row < m_numRows; row++)
    {
      for (int col = 0; col < m_numCols; col++)
        {
          int frame = m_tileMatrix[arrayIndex];

          // draw only the blocks that are not empty (empty represented by 0)
          // note: this makes the first (0) element of the tile array mostly useless
          if (frame > 0)
            {
              m_tileSet->drawFrameEx(gfx, m_xPos + col * m_tileWidth, m_yPos + row * m_tileHeight, frame, angle);
            }
          arrayIndex++;
        }
    }
}


void Grid::draw(Graphics *gfx, int x, int y)
{
    // keep track of position in array to copy memory directly without call to getTile()
  int arrayIndex = 0;
  
  // iterate through all tiles of the grid and draw
  for (int row = 0; row < m_numRows; row++)
    {
      for (int col = 0; col < m_numCols; col++)
        {
          int frame = m_tileMatrix[arrayIndex];

          // draw only the blocks that are not empty (empty represented by 0)
          // note: this makes the first (0) element of the tile array mostly useless
          if (frame > 0)
            {
              m_tileSet->drawFrame(gfx, x + col * m_tileWidth, y + row * m_tileHeight, frame);
            }
          arrayIndex++;
        }
    }
}


void Grid::merge(Grid* persistentGrid, int row, int col)
{
  // keep track of position in array to copy memory directly without call to getTile()
  int arrayIndex = 0;
  
  // iterate row by row
  for (int r = row; r < row + m_numRows; r++)
    {
      // iterate column by column
      for (int c = col; c < col + m_numCols; c++)
        {
          // must check bounds so that we only merge overlapping tiles
          if (c >= 0 && c <= persistentGrid->getNumCols() && r >= 0 && r <= persistentGrid->getNumRows())
            {
              // copy tiles from this grid to destination but ignore empty blocks (val = 0)
              if (m_tileMatrix[arrayIndex] > 0)
                {
                  persistentGrid->setTile(r, c, m_tileMatrix[arrayIndex]);
                }
            }
          
          arrayIndex++;
        }
    }
}

// swap an entire row for the contents of of another
void Grid::swapRows(int row1, int row2)
{
  std::swap_ranges(&m_tileMatrix[row1 * m_numCols], &m_tileMatrix[row1 * m_numCols + m_numCols]
                   , &m_tileMatrix[row2 * m_numCols]);
}


// TODO implement direction to reverse a rotation
void Grid::rotate(int direction)
{
  if(direction >= 0)
    {
      for (int pass = 0; pass < m_numRows / 2; pass++)
        {
          int nColSwaps = m_numCols - 2*pass - 1;

          for (int j = 0, last = nColSwaps + pass; j < nColSwaps; j++, last--)
            {
              int curr = j + pass;
              
              std::swap(m_tileMatrix[pass * m_numCols + curr]
                        , m_tileMatrix[curr * m_numCols + nColSwaps + pass]);
              
              std::swap(m_tileMatrix[pass * m_numCols + curr]
                        , m_tileMatrix[(nColSwaps + pass) * m_numCols + last]);
              
              std::swap(m_tileMatrix[pass * m_numCols + curr]
                        , m_tileMatrix[last * m_numCols + pass]);
            }
        }
    }
  else // direction is negative
    {
      for (int pass = 0; pass < m_numRows / 2; pass++)
        {
          int nColSwaps = m_numCols - 2*pass - 1;

          for (int j = 0, last = nColSwaps + pass; j < nColSwaps; j++, last--)
            {
              int curr = j + pass;

              std::swap(m_tileMatrix[pass * m_numCols + curr]
                        , m_tileMatrix[last * m_numCols + pass]);

              std::swap(m_tileMatrix[pass * m_numCols + curr]
                        , m_tileMatrix[(nColSwaps + pass) * m_numCols + last]);
              
              std::swap(m_tileMatrix[pass * m_numCols + curr]
                        , m_tileMatrix[curr * m_numCols + nColSwaps + pass]);
            }
        }
    }
}


void Grid::free()
{
  // deallocate grid memory
  delete[] m_tileMatrix;
}


void Grid::setTile(int row, int col, int tile)
{
  m_tileMatrix[row * m_numCols + col] = tile;
}


void Grid::setRowTiles(int row, int tile)
{
  for (int col = 0; col < m_numCols; col++)
    {
      setTile(row, col, tile);
    }
}


void Grid::setColTiles(int col, int tile)
{
  for (int row = 0; row < m_numRows; row++)
    {
      setTile(row, col, tile);
    }
}


void Grid::setAllTiles(int tile)
{
    // initialize all tiles to element (0 used to indicate empty block)
  for (int i = 0; i < m_numRows * m_numCols; i++)
    {
      m_tileMatrix[i] = tile;
    }
}

//TODO debugging function - delete
void Grid::print()
{
  for (int i = 0; i < m_numRows; i++)
    {
      printf("[ ");
        
      for (int j = 0; j < m_numCols; j++)
        printf("%d ", m_tileMatrix[i * m_numCols + j]);

      printf("\b]\n");
    }
}

//?? should this be inlined
int Grid::getTile(int row, int col)
{
  // make sure the requested tile is within the boundaries of the grid
  if (row >= 0 && row < m_numRows && col >= 0 && col <= m_numCols)
    {
      return m_tileMatrix[row * m_numCols + col];
    }
  else // the requested tile is outside the boundaries
    {
      //TODO either check for boundaries project-wide or remove existing non-necessary checks
      //printf("the requested tile is outside the boundaries\n");
      //printf("row: %d, col: %d\n", row, col);
      return 0;
    }

  // printf("returned value: %d\n", m_tileMatrix[row * m_numCols + col]);
  // return m_tileMatrix[row * m_numCols + col];
}

bool Grid::isTileSolid(int row, int col)
{
  return (getTile(row, col) > 0);
}
