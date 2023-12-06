//
// endState.h
//
// the game over state
#ifndef END_STATE_H_
#define END_STATE_H_


#include "core/gameState.h"
#include "core/graphics.h"
#include "core/image.h"
#include "core/input.h"
// #include "runState.h"
#include "core/outlineFont.h"

// #include "core/stateManager.h"
#include "core/textOverlay.h"
#include "core/gamepad.h"

#include "textPrompt.h"
#include <map>
#include <string>
// #include <sstream>

class RunState;

class EndState : public GameState
{
private:
  Graphics* p_graphics;
  Input* p_input;
  Gamepad* p_gamepad;
  OutlineFont font; // to render on screen instructions
  Image background;
  Image promptForName; // image to tell user to enter name 

  // changed to gamestate
  RunState* m_prevState;  // to render the background of with playfield of runstate
  TextOverlay* p_stats;
  TextPrompt nameEntry;
  bool displayLeaderboard;
  bool m_getPlayerName; // true if current player score is good enough for leaderboard
  int m_score;
  //  std::string m_playerName;
  std::multimap<int, std::string> hiScores;
  
  //TODO find out why one would initialize this way--I think it has to do with efficiency
  // hiScore.str(std::string());
  // hiScore << "High Score: " << score;
  void loadLeaderboard();
  void updateLeaderboard();
  
public:
  bool init(Graphics* gfx, Input* in, Gamepad* gamepad, StateManager* manager
            , TextOverlay* stats, RunState* prevState);
  void reset(int score);
  void free();

  virtual void update();
  virtual void draw(Graphics* gfx);
};
#endif
