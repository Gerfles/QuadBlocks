// --------------------------------------------------------------------------------
// image.cpp
// ________________________________________________________________________________
#include <string>
#include "SDL2/SDL_render.h"
#include "graphics.h"
#include "image.h"

Image::Image()
{
  // initialize
  m_texture = NULL;
  width = 0;
  height = 0;
  frameWidth = 0;
  frameHeight = 0;
}

Image::~Image()
{
  // deallocate
  free();
}

bool Image::load(Graphics* g, std::string path)
{
  // get rid of pre-existing texture if it exist so we avoid memory leaks
  free();
  
  // load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL)
    {
      SDL_Log( "Unable to load image: %s!\nSDL_image Error: %s", path.c_str(), IMG_GetError());
      return false;
    }

   // TODO allow for color-keyed alpha pixel exclusion to be passed in the load function. That way
   // we can allow certain images to have alpha keys and others just use the alpha of PNG format
   // color key image to get rid of background pixels (DISABLED FOR NOW)
   // SDL_SetColorKey(loadedSurface, SDL_TRUE,
   //       	  SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

  
  // the final texture
  // create texture from surface pixels
  //SDL_Texture* newTexture = NULL;
  //  newTexture = SDL_CreateTextureFromSurface(g->getBackBuffer(), loadedSurface);
  m_texture = SDL_CreateTextureFromSurface(g->getBackBuffer(), loadedSurface);
  
  if (m_texture == NULL)
    {
      printf("unable to create texture from surface! SDL Error: %s\n"
	     , SDL_GetError());
      return false;
    }

  // get image dimensions
  width = loadedSurface->w;
  height = loadedSurface->h;

  // get rid of old loaded surface
  SDL_FreeSurface(loadedSurface);

  // return success
  //m_texture = newTexture;
  return m_texture != NULL;
}


// loads a spritesheet but tells image the dimensions of each frame
bool Image::load(Graphics* g, std::string path, int w, int h)
{
  // load using normal load function then set the frame dims.
  if (load(g, path))
    {
      frameWidth = w;
      frameHeight = h;
      numColumns = width / frameWidth;
      return true;
    }
  
  return false;
}


void Image::loadFromTexture(SDL_Texture* texture, int w, int h)
{
  //  make sure we don't overwrite a texture and orphan the old
  free();
  m_texture = texture;
  width = w;
  height = h;
}


void Image::free()
{
  // free texture if it exists
  if (m_texture != NULL)
    { 
      SDL_DestroyTexture(m_texture);
      m_texture = NULL;
      width = 0;
      height = 0;
    }
}


void Image::draw(Graphics* g, int x, int y, SDL_Rect* clip)
{
  // set rendering space
  SDL_Rect renderQuad = {x, y, width, height};
  if (clip != NULL)
    {
      renderQuad.w = clip->w;
      renderQuad.h = clip->h;
    }
  // render to screen
  SDL_RenderCopy(g->getBackBuffer(), m_texture, clip, &renderQuad);
}


// define this as a new function so that we don't have the overhead of calling renderEx
// when we don't need rotation or flipping
void Image::drawEx(Graphics* g, int x, int y, SDL_Rect* clip, double angle,
			SDL_Point* center, SDL_RendererFlip flip)
{
  // set rendering space and render to screen
  SDL_Rect renderQuad = {x, y, width, height};
  if (clip != NULL)
    {
      renderQuad.w = clip->w;
      renderQuad.h = clip->h;
    }
  // render to screen
  SDL_RenderCopyEx(g->getBackBuffer(), m_texture, clip, &renderQuad, angle, center, flip);
}


// draw a particular frame (cell) of the loaded image
void Image::drawFrame(Graphics* g, int x, int y, int frame, int w, int h)
{
  SDL_Rect destRect = {x, y, frameWidth, frameHeight};
  // set rendering space
  if ( w != -1 && h != -1)
    {
      destRect.w = w;
      destRect.h = h;
    }
  
  SDL_Rect sourceRect;
  sourceRect.x = (frame % numColumns) * frameWidth;
  sourceRect.y = (frame / numColumns) * frameHeight;
  sourceRect.w = frameWidth;
  sourceRect.h = frameHeight;


  SDL_RenderCopy(g->getBackBuffer(), m_texture, &sourceRect, &destRect);
}


// renders a specific frame of a spritesheet using extended functionallity
void Image::drawFrameEx(Graphics* g, int x, int y, int frame, double angle,
		   SDL_Point* center, SDL_RendererFlip flip)
{
  // set rendering space and render to screen
  SDL_Rect destRect = {x, y, frameWidth, frameHeight};
  
  SDL_Rect sourceRect;
  sourceRect.x = (frame % numColumns) * frameWidth;
  sourceRect.y = (frame / numColumns) * frameHeight;
  sourceRect.w = frameWidth;
  sourceRect.h = frameHeight;


  SDL_RenderCopyEx(g->getBackBuffer(), m_texture, &sourceRect, &destRect, angle, center, flip);
}


/*** set alpha modulation and blending mode ***/
void Image::setAlpha(Uint8 alpha, SDL_BlendMode blending)
{
  // // set blending function
  SDL_SetTextureBlendMode(m_texture, blending);
  
  // // modulate texture alpha
  SDL_SetTextureAlphaMod(m_texture, alpha);
}


void Image::modColor(Uint8 r, Uint8 g, Uint8 b)
{
  SDL_SetTextureColorMod(m_texture, r, g, b);
}

