//
// game.h
//
#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL_ttf.h>
#include <sstream>
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "gamepad.h"


class Game
{
private:
  Graphics graphics;
  Input input;
  Gamepad gamepad;
  Audio audio;
  int fps;
  bool isDone;
public:
  Game();
  ~Game();
  bool initSystem(int width, int height, bool fullscreen);
  void freeSystem();

  void setFPS(int f);
  unsigned int getTicks();
  void delay(int ticks);

  void run();
  void end() { isDone = true; }
	
  // Getter functions
  Graphics* getGraphics() { return &graphics; }
  Input* getInput() { return &input; }
  Audio* getAudio() { return &audio; }
  Gamepad* getGamepad() {return &gamepad; }

  //?? make pure virtual functions?
  virtual bool init();
  virtual void free();
  virtual void update();
  virtual void draw(Graphics* gfx);
};
#endif
