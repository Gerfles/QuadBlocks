//
// gamepad.cpp
//
#include "gamepad.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_joystick.h"
#include "SDL2/SDL_log.h"
#include "SDL2/SDL_timer.h"
#include <cstring>

// TODO create a function that allows you to check multiple isButtonhit etc. at once
// instead of calling isbuttonHit etc for every button you want to check

Gamepad::Gamepad()
{
  // set the controller to NULL
  gameController = NULL;
  whichController = -1;

  // set buttons and axis to zero
  memset(buttonStates1, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
  memset(buttonStates2, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
  memset(axisValues, 0, sizeof(Uint8)*SDL_CONTROLLER_AXIS_MAX);

  currButtonStates = buttonStates1;
  prevButtonStates = buttonStates2;
}


//??? should consider adding this to the constructor
bool Gamepad::init()
{
// make sure to initialze the joystick for input within SDL
  if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1)
    {
      if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
        {
          SDL_Log("SDL could not initialize Game Controller subsystem!");
          return false;
        }
    }
  
  // load the gamecontrollerdb.txt and check if there was any problem
  // gamecontrollerdb.txt will add mapping to its list of compatible controllers
  int numControllers = SDL_GameControllerAddMappingsFromFile("maps/gamecontrollerdb.txt");
   //int numControllers = SDL_GameControllerAddMappingsFromFile("maps/ps4controller.txt");
  
  if (numControllers == -1)
    {
      SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Error loading database [%s]", SDL_GetError());
      return false;
    }

  // TODO allow multiple controllers to be used
  int numGamepads = 0;

  // count how many controllers there are
  for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
      if (SDL_IsGameController(i))
        {
          numGamepads++;
          // TODO this is a patch for using only one controller
          whichController = i;
          gameController = SDL_GameControllerOpen(i);
        }
    }
  
  // ignore the controller events so we do our own update outside event loop
  SDL_GameControllerEventState(SDL_IGNORE);
  return true;
}


void Gamepad::update()
{
   // exit if there are no controllers attached
  if (gameController == NULL)
  {
     return;
  }

   //TODO change Uint to the thing it copies--> https://stackoverflow.com/questions/4729046/memcpy-vs-for-loop-whats-the-proper-way-to-copy-an-array-from-a-pointer

   //copy the buttons state to the previous frame buffer
   //memcpy(&prevButtonStates, &buttonStates, sizeof(buttonStates[0]) * SDL_CONTROLLER_BUTTON_MAX);
   //memcpy(&prevButtonStates, &buttonStates, sizeof(buttonStates[0]) * SDL_CONTROLLER_BUTTON_MAX);
  
  std::swap(currButtonStates, prevButtonStates);

   // update the SDL controller info
  SDL_GameControllerUpdate();

   // obtain the current button values
  for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; ++b)
  {
    currButtonStates[b] = SDL_GameControllerGetButton(gameController, (SDL_GameControllerButton)b);
  }

   // obtain the current axis value
  for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; ++a)
  {
    axisValues[a] = SDL_GameControllerGetAxis(gameController, (SDL_GameControllerAxis)a);
  }
}

// BUG controller constantly being dropped and then added. Seems to have started with SDL
// update. Should look for a solution but also streamline adding and removing controllers to the
// point where even if one were to do that, the game would not interrupt

// handle adding and removing controllers during gameplay
void Gamepad::receiveEvent(const SDL_Event &event)
{
  switch(event.type)
    { // controller added event 
    case SDL_CONTROLLERDEVICEADDED:
      {
        SDL_Log("Controller added");
        // if there is not already a controller attached
        if (gameController == NULL)
          {
            // open the controller
            whichController = event.cdevice.which;
            gameController = SDL_GameControllerOpen(whichController);

            // set the memory to 0 to avoid problems with the previous added controllers
            // memset(currButtonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
            // memset(prevButtonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
            // memset(axisValues, 0, sizeof(Uint8) * SDL_CONTROLLER_AXIS_MAX);
          }
        break;
      }
      // controller removed event
    case SDL_CONTROLLERDEVICEREMOVED:
      {
        SDL_Log("Controller removed");
      // TODO allow multiple controllers to be connected
        // check if it's the same controller
          if (whichController == event.cdevice.which)
          {
            whichController = -1;
            gameController = NULL;
          }
        break;
      }
    }
}

// check to see if any new button is hit
// TODO implement this in a smarter way, such as a wrapper class that contains the buttons we are
// seeking to check and if that's the case it stores the state of only the keys we care about
bool Gamepad::anyButtonHit() const 
{
  for (int button = 0; button < MAX_BUTTONS; button++)
    {
      if (currButtonStates[button] && !prevButtonStates[button])
        {
          return true;
        }
    }
  
  return false;
}


bool Gamepad::isButtonHit(const GamepadButton button) const
{
  return (currButtonStates[button] && !prevButtonStates[button]);
}


bool Gamepad::isButtonPressed(const GamepadButton button) const 
{
  return (currButtonStates[button]);
}


bool Gamepad::isButtonReleased(const GamepadButton button) const
{
  return (!currButtonStates[button] && prevButtonStates[button]);
}


float Gamepad::getAxisVal(const SDL_GameControllerAxis axis) const
{
  return (axisValues[axis]);
}


void Gamepad::kill()
{
  //TODO figure out close state
  // close game controller
  SDL_GameControllerClose(gameController);
  gameController = NULL;
  // since sdl is initialized with init_everything, we might not be able to do this here
  SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

