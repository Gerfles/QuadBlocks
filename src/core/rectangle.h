//
// rectangle.h
//
#ifndef RECTANGLE_H_
#define RECTANGLE_H_

class Rectangle
{
private:
  int x, y, width, height;
public:
  Rectangle(int ix, int iy, int iwidth, int iheight) : x(ix), y(iy), width(iwidth)
						     , height(iheight) {};
  Rectangle() : x(0), y(0), width(1), height(1) {};
  ~Rectangle() {};
  // alteration methods
  void set(int ix, int iy, int iwidth, int iheight);
  void setPos(int ix, int iy) { x = ix; y = iy; }
  void setSize(int iwidth, int iheight) { width = iwidth; height = iheight; }
  void setX(int ix) { x = ix; }
  void setY(int iy) { y = iy; }
  void setX2(int x2) { x = x2-width+1; } //?? not sure the reason for +1 (maybe div by 0)
  void setY2(int y2) { y = y2-height+1; } //?? not sure the reason for +1
  void setWidth(int iWidth) { width = iWidth; }
  void setHeight(int iHeight) {height = iHeight; }
  void setCenterX(int ix) { x = ix - width/2; }
  void setCenterY(int iy) { y = iy - height/2; }
  // getter methods
  int getX() { return x; }
  int getY() { return y; }
  int getX2() { return (x + width - 1); } //?? again with the 1... starting to seem it is there to simplify the overlaps method... Don't think that would be the case regardless though since the overlapse method compares a rect, which would have it's own buffer
  int getY2() { return (y + height - 1); }
  int getWidth() {return width; }
  int getHeight() { return height; }
  int getCenterX() { return (x + width/2); }
  int getCenterY() { return (y + height/2); }
  // check conditions 
  bool overlaps(Rectangle rect);
  bool isInsideBoundary(int x, int y);
};
#endif
