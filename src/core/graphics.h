//
// graphics.h
//
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <sstream>
#include <string>


class Graphics
{
private:
  // the actual hardware texture
  SDL_Renderer* backBuffer;
  // the window linked to the above backbuffer
  SDL_Window* window;
  // image dimensions
  int screenWidth;
  int screenHeight;

public:
  Graphics();
  ~Graphics();
  
  // initializes the graphics class
  bool init(int winWidth, int winHeight, bool winFullscreen);

  // deallocates texture
  void free();

  // draws a pixel to the screen
  void drawPixel(int x, int y, int r, int g, int b, int alpha = 255);

  void drawLine(int x1, int y1, int x2, int y2, int r, int g, int b
		, int alpha = 255);

  // draws an outline of a AA (axis aligned) rectangle
  void drawRect(int x, int y, int width, int height, int r, int g, int b
		, int alpha = 255);

  // draws an outline of a AA (axis aligned) rectangle
  void drawRect(SDL_Rect* rect, int r, int g, int b, int alpha = 255);

  // draws a solid rectangle
  void fillRect(int x, int y, int width, int height, int r, int g, int b
		, int alpha = 255);

  // draws a solid rectangle
  void fillRect(SDL_Rect* rect, int r, int g, int b, int alpha = 255);

  // clear backbuffer and fill with given color 
  void clear();

  // render the backbuffer to the screen
  void flip();
  
  // geter functions
  int getScreenWidth() { return screenWidth; }
  int getScreenHeight() { return screenHeight; }
  SDL_Renderer* getBackBuffer () { return backBuffer; }
  SDL_Window* getWindow () { return window; }
};
#endif
