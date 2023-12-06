//
// image.h
//
#ifndef IMAGE_H_
#define IMAGE_H_

#include <SDL2/SDL_image.h>
#include <string>
#include "graphics.h"


// Image wrapper class
class Image
{
private:
  // the actual hardware texture
  SDL_Texture* m_texture;

  // image dimensions
  int width;
  int height;
  int frameWidth;
  int frameHeight;
  int numColumns;
  void createFontTexture();
public:
  // initializes variables
  Image();

  // deallocates memory
  ~Image();

  // loads image at specified paths
   bool load(Graphics* g, std::string path);

  // loads a spritesheet but tells image the dimensions of each frame
  bool load(Graphics* g, std::string path, int frameWidth, int frameHeight);

  // save an SDL_texture to image class
  void loadFromTexture(SDL_Texture* texture, int w, int h);
    
  // deallocates texture
  void free();

  // renders texture at given point
  void draw(Graphics* g, int x, int y, SDL_Rect* clip = NULL);

  // renders a specific frame of a spritesheet
  void drawFrame(Graphics* g, int x, int y, int frame, int w = -1, int h = -1);
  
  // draw using extended functionallity
  void drawEx(Graphics* g, int x, int y, SDL_Rect* clip = NULL,
	      double angle = 0.0, SDL_Point* center = NULL,
	      SDL_RendererFlip flip = SDL_FLIP_NONE);

  // renders a specific frame of a spritesheet using extended functionallity
  void drawFrameEx(Graphics* g, int x, int y, int frame,
	      double angle = 0.0, SDL_Point* center = NULL,
	      SDL_RendererFlip flip = SDL_FLIP_NONE);
  
  // set alpha modulation
  void setAlpha(Uint8 alpha, SDL_BlendMode blending);
  
  // alters dimensions of image (for stretching purposes)
  void setDimensions(int w, int h) {width = w; height = h;}

  //?? the functionallity of this function may already be provided by the above function
  void setFrameSize(int w, int h) {frameWidth = w; frameHeight = h;}

  // modulate the color of the texture
  void modColor(Uint8 r, Uint8 g, Uint8 b);
  
  //
  bool isLoaded() { return (m_texture != NULL); }
  
  // geter functions
  int getWidth() { return width; }
  int getHeight() { return height; }
  int getFrameWidth() { return frameWidth; }
  int getFrameHeight() { return frameHeight; }
  
};
#endif
