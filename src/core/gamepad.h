// gamepad.h
//
// handle input from joystick controller

#include "SDL2/SDL.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_joystick.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_stdinc.h"
#include<SDL2/SDL.h>


#ifndef GAMEPAD_H_
#define GAMEPAD_H_

// rename SDL_CONTROLLER_... to make code more readable
typedef enum
  {
    INVALID_BUTTON = -1,
    A_BUTTON,
    B_BUTTON,
    X_BUTTON,
    Y_BUTTON,
    BACK_BUTTON,
    GUIDE_BUTTON,
    START_BUTTON,
    LEFT_STICK,
    RIGHT_STICK,
    LEFT_SHOULDER,
    RIGHT_SHOULDER,
    DPAD_UP,
    DPAD_DOWN,
    DPAD_LEFT,
    DPAD_RIGHT,
    MAX_BUTTONS
  } GamepadButton;


class Gamepad
{
private:
  //const int JOYSTICK_DEAD_ZONE = 8000;
  SDL_GameController* gameController; // game controller 1 handler
  Sint32 whichController;
  // information about the state of the controller
  Uint8 buttonStates1[SDL_CONTROLLER_BUTTON_MAX];
  Uint8 buttonStates2[SDL_CONTROLLER_BUTTON_MAX];
  Uint8* currButtonStates;
  Uint8* prevButtonStates;
  float axisValues[SDL_CONTROLLER_AXIS_MAX];
public:
  // constructor
  Gamepad();

  // object controll functions
  bool init();
  void update();
  void kill();

  // to receive the connect and disconnect event
  void receiveEvent(const SDL_Event& event);

  // functions to retrieve the state of the buttons
  bool isButtonHit(const GamepadButton button) const;
  bool anyButtonHit() const;
  bool isButtonPressed(const GamepadButton button) const;
  bool isButtonReleased(const GamepadButton button) const;
  // specialized function to only return true a certain number of times per sec
  //  bool isButtonHitDelay(const GamepadButton button, Uint32 delayTime); 

  // function to get the current value of the axis
  float getAxisVal(const SDL_GameControllerAxis axis) const;
 };

#endif
