//
// runState.h
//
#ifndef RUNSTATE_H_
#define RUNSTATE_H_


#include "SDL2/SDL_stdinc.h"
#include "core/gameState.h"
#include "core/graphics.h"
#include "core/audio.h"
#include "core/image.h"
#include "core/input.h"
#include "core/music.h"
#include "core/grid.h"
#include "core/rasterFont.h"
#include "core/stateManager.h"
#include "core/timer.h"
#include "pauseState.h"
#include "endState.h"
#include "pieces.h"
#include "core/parallax.h"
#include "core/gamepad.h"
// debugging
#include "gameMath.h"
#include "optionState.h"
#include "textOverlay.h"
#include <string>
#include <deque>

// declare a Pausestate class since this class definition requires the PauseState and
// the Pausestate requires a runstate to be defined
//class EndState;
//class PauseState;


class RunState : public GameState
{
private:
  // constant parameters
  const static int BLOCK_DIM = 32;
  const static int SM_BLOCK_DIM = 24;
  
  // the repeat delay time to wait before another key can be registered
  const static int REPEAT_MOVE_DELAY = 200;
  // the amount of time after the piece has landed before it becomes immobile
  const static Uint32 BONUS_MANUEVER_TIME = 350;
  // the length of time the bricks stay white before cleared
  const static Uint32 SPECIAL_FX_TIME = 100;
  
  // stats
  int m_screenWidth;
  int m_screenHeight;
  int level;
  int totalLines;
  int tetrises;
  int score;
  TextOverlay stats;

  // game timer
  Timer timer;
  Uint32 dropTime;
  Uint32 lastDropTime;
  Uint32 manueverStartTime;
  Uint32 fxStartTime;
  int moveDelay;
  Uint32 keyPressStartTime;
  Uint32 seconds;
  char timerText[10];

  // Image Resources
  Image m_console;
  Image m_tetriminoSprites;
  Image m_playfieldSprites;
  Image m_ghostSprites;
  Image m_miniTetriminos;
  Parallax backgroundImg;
  
  // font for rendering score, level, etc.
  RasterFont statsFont;

  // 
  bool initialized = false;
  
  // Substates to run from runstate
  PauseState m_pauseState;
  EndState m_endState;


  // scene nodes
  Piece* pieces[7];
  Piece* pipelinePieces[7];
  Piece* ghostPieces[7];
  Grid playField;

  Piece* currPiece;
  Piece* ghostPiece;
  // keep track of the rotation of the ghost piece to draw correctly
  int ghostRotateAngle;
  // 0 = next piece, 1-4 = upcoming pieces
  std::deque<int> pieceIDs;
  int holdPiece;
  bool canSwap;
  
  // shared resources
  Graphics* 	p_graphics;
  Audio* 	p_audio;
  Input*	p_input;
  Gamepad* 	p_gamepad;
  HUD* 		p_optionsHUD;

  
  //debugging
  int m_matrix[4][4];

  void outputStats();

public:
  RunState();
  bool init(Graphics* graphics, Input* input, Gamepad* gamepad, Audio* audio
            ,int screenWidth, int screenHeight, HUD* optionsHUD);
  void free();
  void update();
  void draw(Graphics* graphics);
  void drawEnvironment();
  void reset();
  void unpauseTimer();
  bool wasInit() { return initialized; }
  int getScore() { return score; }
};

#endif
