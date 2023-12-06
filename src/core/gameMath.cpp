//
// gameMath.cpp
//
#include "gameMath.h"


void printMatrix(int numRows, int matrix[SIZE][SIZE])
{
  using std::cout;
  
  for (int row = 0; row < numRows; row++)
    {
      cout << "[ ";
      
      for (int column = 0; column < numRows; column++)
	{
	  cout << std::setw(3);
	  cout << matrix[row][column] << " ";
	}
      cout << "\b]\n";
    }
}


void rotateMatrix(int numRows, int matrix[SIZE][SIZE])
{
  // could use a single temp variable instead of swap (should do a code speed test)
  
  for (int pass = 0; pass < numRows / 2 ; pass++)
    {
      int colSwaps = numRows - 2*pass - 1;
      for (int j = 0, last = colSwaps + pass; j < colSwaps; j++, last--)
	{
          int current = j + pass;
	  std::swap(matrix[pass][current], matrix[current][colSwaps + pass]);
	  std::swap(matrix[pass][current], matrix[colSwaps + pass][last]);
	  std::swap(matrix[pass][current], matrix[last][pass]);
	}
    }
}


void copyMatrix(int matrix[SIZE][SIZE], Grid* piece)
{
  for (int row = 0; row < SIZE; row++)
    {
      for (int col = 0; col < SIZE; col++)
        {
          matrix[row][col] = piece->getTile(row, col);
        }
    }
}

