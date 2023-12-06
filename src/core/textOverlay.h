// textOverlay.h
//
// used for displaying text in a uniform way with effects, ie. scrolling used in credits
// TODO - make format more forgiving (ignore whitespace, spelling, case, etc.)
//       :::FORMAT:::
/*--------------------------------------------------------------------------------------
font_size [font size in points]
font_file [TTF font name]
screen_offset [x] [y]
color [r] [g] [b]
spacing [number of pixels between lines]
method [SCROLL or BULLET or EMPHASIZE]
speed [pixels per second or number of milliseconds before line effects]
linger [milliseconds to hold after all lines displayed] 0 signifies to linger indefinitely
alignment [LEFT or RIGHT or CENTER]
inverted [TRUE or FALSE]
[then text, eg the following]
Geoff Phillips
 Lead programmer

Person B
 Programmer
 Artist

 etc...
--------------------------------------------------------------------------------------*/

#ifndef TEXTOVERLAY_H_
#define TEXTOVERLAY_H_

#include "outlineFont.h"
#include "graphics.h"
#include "timer.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>


class TextOverlay
{
private:
  // see methods below for descriptions
  enum displayMethod {SCROLL, BULLET, EMPHASIS};
  enum alignmentMode {CENTER, LEFT, RIGHT};
  std::vector<std::string> lines;
  std::string m_filename;
  Timer timer;
  int numLines;
  int fontSize;
  double top; // where to draw the top of the lines
  bool finished; // TRUE when all lines done displaying
  int skipLines; 
  Graphics* graphics_p;
  Uint32 prevTime;
  int numDrawLines; // used to flag how many lines have been drawn
  // user (file) supplied parameters  
  OutlineFont font;
  int offsetX, offsetY;
  int width, height;
  int r, g, b;
  int spacing;
  Uint32 speed;
  int invert;
  int mode;
  int linger;
  int alignment;
  
  // helper functions
  void drawLine(int index, int yPosition);

  // scroll through lines like credits
  void drawScroll();

  // time delayed list that displays like bullet points
  void drawBullet();

  // only draw one line at a time and keep on screen until enough time (speed, in ms) has passed 
  void drawEmphasis();

public:
  const static int TEXT_START_LINE = 11; // text overlay file has 11 lines of data before text to display
  
  bool load(Graphics* gfx, const std::string filename);
  // return -1 for failure and number for how many lines have been drawn
  void clearText();
  void addLine(std::string line);
  int draw();
  void reset();
  void free();
};

#endif

