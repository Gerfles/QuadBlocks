//
// graphics.cpp
//
#include "graphics.h"
#include "SDL2/SDL_render.h"

Graphics::Graphics()
{
  backBuffer = NULL;
}

Graphics::~Graphics()
{
  
}

bool Graphics::init(int winWidth, int winHeight, bool winFullscreen)
{
  screenWidth = winWidth;
  screenHeight = winHeight;

  if (winFullscreen)
    {
      // create fullscreen window
      this->window = SDL_CreateWindow( "SDL Window", SDL_WINDOWPOS_UNDEFINED,
  				 SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
  				 SDL_WINDOW_FULLSCREEN);
    }
  else
    {
      // create windowed window
      this->window = SDL_CreateWindow( "SDL Window", SDL_WINDOWPOS_UNDEFINED
  				 , SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight
  				 , SDL_WINDOW_SHOWN);
    }

  
  std::ostringstream errorMsg;
  //?? wonder if we should always use NULL instead of nullptr?
  if ( window == NULL)
    {
      errorMsg << "Window could not be created! SDL Error: " << SDL_GetError();
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL", errorMsg.str().c_str(), window);
      return false;
    }

  // create renderer for window 
  backBuffer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  if( backBuffer == NULL )
    {
      errorMsg << "Renderer could not be created! SDL Error: " << SDL_GetError();
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL", errorMsg.str().c_str(), window);
      return false;
    }


  //?? still not completely sure this is needed the documentation is not great
  // initialize SDL_image for PNG loading
  int imgFlags = IMG_INIT_PNG;
  if( !(IMG_Init(imgFlags) & imgFlags) )
    {
      printf("SDL_Image could not initialize! SDL_Image Error: %s\n",
  	     IMG_GetError());
    }


  return true;
}

  
// deallocates texture
void Graphics::free()
{
  SDL_DestroyRenderer(backBuffer);
  SDL_DestroyWindow(window);
  backBuffer = NULL;
  window = NULL;
}

// draws a pixel to the screen
void Graphics::drawPixel(int x, int y, int r, int g, int b, int alpha)
{
  if (backBuffer == NULL)
    {
      return;
    }

  SDL_SetRenderDrawColor(backBuffer, r, g, b, alpha);
  SDL_RenderDrawPoint(backBuffer, x, y);
}

void Graphics::drawLine(int x1, int y1, int x2, int y2, int r, int g, int b, int alpha)
{
  SDL_SetRenderDrawColor(backBuffer, r, g, b, alpha); 
  SDL_RenderDrawLine(backBuffer, x1, y1, x2, y2);
}

// draws an outline of a AA (axis aligned) rectangle
void Graphics::drawRect(int x, int y, int width, int height, int r, int g, int b, int alpha)
{
  //?? doesn't appear to work to render alpha levels?
  SDL_SetRenderDrawColor(backBuffer, r, g, b, alpha);

  SDL_Rect destRect = {x, y, width, height};
  
  SDL_RenderDrawRect(backBuffer, &destRect);
}

// draws an outline of a AA (axis aligned) rectangle
void Graphics::drawRect(SDL_Rect* rect, int r, int g, int b, int alpha)
{
  SDL_SetRenderDrawColor(backBuffer, r, g, b, alpha);
  SDL_RenderDrawRect(backBuffer, rect);
}

// draws a solid rectangle
void Graphics::fillRect(int x, int y, int width, int height, int r, int g, int b, int alpha)
{
  SDL_SetRenderDrawColor(backBuffer, r, g, b, alpha);

  SDL_Rect destRect = {x, y, width, height};

  SDL_RenderFillRect(backBuffer, &destRect);
}

// draws a solid rectangle
void Graphics::fillRect(SDL_Rect* rect, int r, int g, int b, int alpha)
{
  SDL_SetRenderDrawColor(backBuffer, r, g, b, alpha);
  SDL_RenderFillRect(backBuffer, rect);
}

// clear backbuffer and fill with given color
//?? may not be needed when using a renderer as opposed to a SDL_Surface but we'll
// include it anyways in case it's use proves valuable
void Graphics::clear()
{
  if (backBuffer == NULL)
    {
      return;
    }
  SDL_RenderClear(backBuffer);
}

// render the backbuffer to the screen
void Graphics::flip()
{
  //  SDL_RenderClear(backBuffer);
   SDL_RenderPresent(backBuffer);
}

