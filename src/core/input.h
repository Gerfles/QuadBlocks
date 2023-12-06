//
// input.h
//
#ifndef INPUT_H_
#define INPUT_H_

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_scancode.h"
#include "timer.h"
#include<SDL2/SDL.h>
#include <string>


class Input
{
private:
  // internal storage of all the keyboard key states 
  bool keyStates1[SDL_NUM_SCANCODES];
  bool keyStates2[SDL_NUM_SCANCODES];
  bool* currKeyStates;
  bool* prevKeyStates;
  
  bool mouseKeys[3];
  bool prevMouseKeys[3];

  int mouseX;
  int mouseY;

  std::string* p_text; // pointer to string for storing text input data
  bool m_hasTextUpdated; // signifies if text input has been received
  Uint32 mouseState;
public:
  //?? perhaps forego these to use SDL_BUTTON(x)
  static const int MOUSE_LEFT = 1;
  static const int MOUSE_MIDDLE = 2;
  static const int MOUSE_RIGHT = 3;

  void init();
  void update();
  void kill();

  bool keyDown(int key);
  bool keyHit(int key);
  bool keyUp(int key);

  bool mouseDown(int key);
  bool mouseHit(int key);
  bool mouseUp(int key);

  void setMousePos(SDL_Window* win, int x, int y);
  void hideCursor(bool hide = true);

  int getMouseX() { return mouseX; }
  int getMouseY() { return mouseY; }

  // text input function
  void enableTextInput(std::string* text);
  // -1 = input no longer active, 1 = text input has been updated, 0 = input doesn't need updated;
  // -Note that text input status will change the internal variable that it has been updated,
  // meaning that the status returned is the status SINCE the last call to the function
  int textInputStatus();
  // reset our variable to signal that text has been rendered and can start sending signal of no update
  void receiveEvent(SDL_Event& event);
};
#endif
