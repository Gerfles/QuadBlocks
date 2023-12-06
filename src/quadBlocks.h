// quadblocks.h
//
//

// BUG's

// displays top scores twice
// in endstate that cause high scores to clobber
// sliders don't work properly with highlighting while using mouse and keyboard

//TODO's

// add keyboard functionality to slider in menu
// add buttons for entering and exiting into pause menu
// make textOverlay work like textPrompt to not redraw text
// maybe pass parallax to each gamestate instead of creating new 
// give bonus time only if a key is being pressed
// simplify the ghost sprite sheet
// make mouse work if used on main menu state
// add support for multiple gamepads (even for this game (alternate piece placement))
// fix rotating sprites when back spinning
// make sure game states only play certain music
// clean up code and remove unused variables
// create new colored sprites for new levels
// get better music & sound effects
// create a consistent policy for dealing with passing graphics to init() and calling it from draw()
// update scoring methods


// Potential Upgrades (FUTURE)

// Let holding L+R swap focus to the shadow piece. While holding L+R, player can place the shadow
// anywhere in the game board while the current piece continues to drop unaffected. This would allow
// the piece to fill in any gaps that might be left present but if the user releases L+R and the
// block is not in a valid position, then a negative bonus would occur (end of game, level advances,
// garbage blocks dropped, etc...)
// allow 3D(ish) piece-bringing-out method (gained by scoring tetrises)
// create a splash screen for the title that has animated blocks falling
// establish a github
// speed up the speed-up. 

// TODO from SDL book
// implement a singleton
/* for vector for loops that run through the vector, use the size_type operator i.e.

   for (std::vector<GameObject*>::size_type i = 0; i != m_gameObjects.size(); i++)
   {
       m_gameObjects[i]->draw(m_pRenderer);
   }
   
*/
// determine if input could be better implemented (do we need to buffer the input)?
// combine input classes into one input and maybe make a singleton
// for the callback function using sound when sound stops--maybe a singleton could make that work
// make sure to make templated functions all virtual
/* see if there would be a reason to implement the FSM as a vector instead of a stack
   one reason might be to completely remove all other states and add a new one to the stack
   for instance if we want a quit feature to go to the main menu and to a add the menu as a new
   frame instead of removing the stack down to the main menu
 */ 
// call init funtion of each game state when it is added to the state manager
// check button operations according to note #5 in SDL book
// implement a vector2D class and alter things that have x,y to now have vector
// make sure we pump events before buffering input
// make an indent function that indents all pages in emacs


#ifndef QUADBLOCKS_H_
#define QUADBLOCKS_H_

#include "core/game.h"
#include "core/stateManager.h"
#include "mainMenuState.h"


class Tetris : public Game
{
private:
  const static int SCREEN_WIDTH = 1024;
  const static int SCREEN_HEIGHT = 768;
  StateManager stateManager;
  MainMenuState mainMenuState;
public:
  Tetris() {};
  ~Tetris() {};
  bool init();
  void free();
  void update();
  void draw(Graphics* gfx);
};

#endif
