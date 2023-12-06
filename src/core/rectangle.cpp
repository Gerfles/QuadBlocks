//
// rectangle.cpp
//
#include "rectangle.h"

void Rectangle::set(int ix, int iy, int iwidth, int iheight)
{
  x = ix;
  y = iy;
  width = iwidth;
  height = iheight;
}


// return true if the this rectangle overlaps the passed rectangle
bool Rectangle::overlaps(Rectangle rect)
{
  //  see if left side of this rectangle is to the right of rect
  if (x >= rect.getX2())
    {
      // return false since this case precludes any overlapping
      return false;
    }
  //  see if right side of this rectangle is to the left of rect
  if (getX2() <= rect.x)
    {
      return false;
    }
  //  see if top of this rectangle is bellow bottom of rect
  if (y >= rect.getY2())
    {
      return false;
    }
  //  see if bottom of this rectangle is above bottom of rect
  if (getY2() <= rect.y)
    {
      return false;
    }

  // since we have gotten this far, we haven't been able to find any of
  // the sides such that they are not overlapping so we must have a collision
  return true;
}


bool Rectangle::isInsideBoundary(int ix, int iy)
{
  if (ix >= x && iy >= y && ix <= getX2() && iy <= getY2())
    {
      return true;
    }

  return false;
}
