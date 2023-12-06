//
//gameMath.h
//
// - a place to store mathematics!
#include <iostream>
#include <iomanip>
#include "grid.h"

static const int SIZE = 4;

void printMatrix(int numRows, int matrix[SIZE][SIZE]);
void rotateMatrix(int numRows, int matrix[SIZE][SIZE]);
void copyMatrix(int matrix[SIZE][SIZE], Grid* piece);


