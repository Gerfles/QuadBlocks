//
// runstate.cpp
//
#include "runState.h"
#include "SDL2/SDL_mixer.h"
#include "parallax.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_timer.h"
#include "core/graphics.h"
#include "core/stateManager.h"
#include "pieces.h"
#include "optionState.h"
#include "textOverlay.h"
#include "assetList.h"
#include <cstdio>
#include <fstream>
#include <ios>
#include <ostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>


RunState::RunState()
{
  initialized = false;
}


bool RunState::init(Graphics* graphics, Input* input, Gamepad* gamepad, Audio* audio
                    ,int screenWidth, int screenHeight, HUD* optionsHUD)
{
  p_graphics = graphics;
  p_input = input;
  p_gamepad = gamepad;
  p_audio = audio;
  p_optionsHUD = optionsHUD;

  m_screenWidth = p_graphics->getScreenWidth();
  m_screenHeight = p_graphics->getScreenHeight();

  // load the parallaxing background
  std::vector<std::string> filenames = {"graphics/layer1.png", "graphics/layer2.png", "graphics/layer3.png"
    , "graphics/layer4Lights.png", "graphics/layer4.png", "graphics/layer5.png"
    , "graphics/layer6.png", "graphics/layer7Lights.png", "graphics/layer7.png"
    , "graphics/layer8.png", "graphics/layer9.png"};

  backgroundImg.load(graphics, filenames, 0, 5, Parallax::RIGHT);

  // load console image
  if (!m_console.load(p_graphics, "graphics/tetris_console.png"))
    return false;
  
  // load tetrimino images spritesheet
  if (!m_tetriminoSprites.load(p_graphics, "graphics/tetrimino_sprites.png", BLOCK_DIM, BLOCK_DIM))
    return false;
  
  // load playfield images spritesheet
  if (!m_playfieldSprites.load(p_graphics, "graphics/playfield_sprites.png", BLOCK_DIM, BLOCK_DIM))
    return false;
  
  // load the spritesheet for the ghost pieces
  if (!m_ghostSprites.load(p_graphics, "graphics/ghost_sprites.png", BLOCK_DIM, BLOCK_DIM))
    return false;
  
  // load the images for the pipeline (side bars) pieces
  if (!m_miniTetriminos.load(p_graphics, "graphics/mini_tetriminos.png", SM_BLOCK_DIM, SM_BLOCK_DIM))
    return false;

  // initialize end of game state
  if (!m_endState.init(p_graphics, p_input, p_gamepad, getManager(), &stats, this))
    return false;

  // initialize the pause state  
  if (!m_pauseState.init(p_input, p_gamepad, getManager(), this, p_optionsHUD))
    return false;
  
  // shuffle the audio tracks and play
  int tracks[4] = {1,2,3,4};
  p_audio->music->shuffleSongs(tracks, 4);

  // load the stats textoverlay file
  if(!stats.load(p_graphics, "text/stats.ovl"))
    return false;
  
  //load rasterized font
  if (!statsFont.load(p_graphics, "text/kromasky.png", 16))
    return false;

  // create tetris playfield - top row is for gameover detection
  playField.init(21, 10, BLOCK_DIM, BLOCK_DIM, &m_playfieldSprites);

  // center in screen
  playField.setX(m_screenWidth / 2  -  playField.getWidth() / 2);
  
  // field is 1 block heights down from the top but will appear to be 2 since row 0 is buffer
  playField.setY(1*BLOCK_DIM + 25);

  //TODO take off the heap
  // initialize tetriminos
  pieces[0] = new L_piece(&m_tetriminoSprites, &playField, BLOCK_DIM);
  pieces[1] = new S_piece(&m_tetriminoSprites, &playField, BLOCK_DIM);
  pieces[2] = new Z_piece(&m_tetriminoSprites, &playField, BLOCK_DIM);
  pieces[3] = new Line_piece(&m_tetriminoSprites, &playField, BLOCK_DIM);
  pieces[4] = new Square_piece(&m_tetriminoSprites, &playField, BLOCK_DIM);
  pieces[5] = new T_piece(&m_tetriminoSprites, &playField, BLOCK_DIM);
  pieces[6] = new BackL_piece(&m_tetriminoSprites, &playField, BLOCK_DIM);

  // initialize un-rotatable tetriminos for HUD use
  // TODO maybe don't associate these pieces with a bounding grid
  pipelinePieces[0] = new L_piece(&m_miniTetriminos, &playField, SM_BLOCK_DIM);
  pipelinePieces[1] = new S_piece(&m_miniTetriminos, &playField, SM_BLOCK_DIM);
  pipelinePieces[2] = new Z_piece(&m_miniTetriminos, &playField, SM_BLOCK_DIM);
  pipelinePieces[3] = new Line_piece(&m_miniTetriminos, &playField, SM_BLOCK_DIM);
  pipelinePieces[4] = new Square_piece(&m_miniTetriminos, &playField, SM_BLOCK_DIM);
  pipelinePieces[5] = new T_piece(&m_miniTetriminos, &playField, SM_BLOCK_DIM);
  pipelinePieces[6] = new BackL_piece(&m_miniTetriminos, &playField, SM_BLOCK_DIM);

  // load the ghost pieces (used to show resting position of dropped piece)
  ghostPieces[0] = new L_ghost(&m_ghostSprites, &playField, BLOCK_DIM);
  ghostPieces[1] = new S_ghost(&m_ghostSprites, &playField, BLOCK_DIM);
  ghostPieces[2] = new Z_ghost(&m_ghostSprites, &playField, BLOCK_DIM);
  ghostPieces[3] = new Line_ghost(&m_ghostSprites, &playField, BLOCK_DIM);
  ghostPieces[4] = new Square_ghost(&m_ghostSprites, &playField, BLOCK_DIM);
  ghostPieces[5] = new T_ghost(&m_ghostSprites, &playField, BLOCK_DIM);
  ghostPieces[6] = new BackL_ghost(&m_ghostSprites, &playField, BLOCK_DIM);

  reset();
  initialized = true;
  return true;
}


void RunState::free()
{
  // free all the resources used 
  backgroundImg.free();
  m_console.free();
  m_tetriminoSprites.free();
  m_playfieldSprites.free();
  m_ghostSprites.free();
  m_miniTetriminos.free();
  m_endState.free();
  m_pauseState.free();  
  stats.free();
  playField.free();

  for (int i = 0; i < 7; i++)
    {
      delete pieces[i];
      delete pipelinePieces[i];
      delete ghostPieces[i];
    }

  initialized = false;
}


void RunState::update()
{
  // make sure we're playing music and if not choose a new track
  p_audio->music->update();

  // check whether to pause game 
  if (p_input->keyHit(SDL_SCANCODE_ESCAPE) || p_gamepad->isButtonHit(START_BUTTON))
    {
      timer.pause();
      getManager()->addState(&m_pauseState);
      // play pause sound
      p_audio->sounds->play(PAUSE_SOUND);
    }

  Uint32 currTime = timer.getTicks();
  
  // remove any lines that have been cleared but turned white
  if (fxStartTime > 0 && currTime - fxStartTime > SPECIAL_FX_TIME)
    {
      currPiece->removeLines();
      fxStartTime = 0;
    }

  // 
  if (!currPiece->hasLanded())
    {
      // if enough time has elapsed or downKey is pressed, drop the piece one row
      if (p_input->keyDown(SDL_SCANCODE_DOWN) || p_gamepad->isButtonPressed(DPAD_DOWN)
          || currTime - lastDropTime >= dropTime)
        {
          // restart step timer by resetting elapsed time
          lastDropTime = currTime;
          if (currPiece->moveDown())
            {
              // play move down sound
            }
        }

      // handle quick drop event
      if (p_input->keyHit(SDL_SCANCODE_UP) || p_gamepad->isButtonHit(DPAD_UP))
        {
          currPiece->drop();
          // play drop sound
          p_audio->sounds->play(DROP_SOUND);
        }
            
      // now that we've moved the piece down check again if it's landed and if so reset buffer time
      if (currPiece->hasLanded())
        {
          manueverStartTime = currTime;
        }
    }
  
  // check if current piece has landed and that enough time has elapsed to grab a new one etc.
  if (currPiece->hasLanded() && currTime - manueverStartTime > BONUS_MANUEVER_TIME)
    {
      // play landed sound
      //p_audio->sounds->play(LAND_SOUND);
      
      // set time to remove the newly cleared blocks
      fxStartTime = currTime;
      
      // paste block into the playfield 
      currPiece->merge(&playField, currPiece->getRow(), currPiece->getCol());

      // remove any lines formed by landed piece
      int clearedLines = currPiece->clearLines();

      // update the line count
      if (clearedLines == 4)
        {
          p_audio->sounds->play(TETRIS_SOUND);
          // keep running total of tetrises
          tetrises++;
        }
      else if (clearedLines > 0)
        {
          p_audio->sounds->play(LINE_CLEAR_SOUND);
        }
      
      // update lines, and level if need be
      totalLines += clearedLines;
      level = totalLines / 10 + 1;
      
      // set the drop time based on level -75 each level with special precaution taken at level 10+
      if (level < 10)
        {
          dropTime = 875 - 90*level;
        }
      else
        {
          dropTime = 335 - 20*level;
        }

      // check for gameover (make sure block has landed completely inside of game board)
      for (int i = 0; i < playField.getNumCols(); i++)
        {
          // check if piece landed outside of playfield (in first row, used as buffer)
          if (playField.getTile(0,i) != 0)
            {
              // play game over sounds
              p_audio->sounds->play(DEATH_SOUND);
              
              // game ended so write final stats and reset endstate with current score
              outputStats();
              m_endState.reset(score);
              getManager()->addState(&m_endState);
              timer.pause();
              return;
            }
        }
      
      // no game over so select the next piece from the queue
      pieceIDs.pop_front();
      currPiece = pieces[pieceIDs.front()];
      ghostPiece = ghostPieces[pieceIDs.front()];
      pieceIDs.push_back(rand() % 7);

      // reset the pieces and the regain ability to hold a new piece
      currPiece->reset();
      ghostPiece->reset();
      ghostRotateAngle = 0;
      canSwap = true;
    }  // make sure current piece hasn't landed yet then determine if it needs to move down
  else
    {
      // handle piece move left events in 2 stages--normal, and repeat key hits
      if (p_input->keyHit(SDL_SCANCODE_LEFT) || p_gamepad->isButtonHit(DPAD_LEFT))
        {
          keyPressStartTime = currTime;

          if (currPiece->moveLeft())
            {
              // play move sound effect only if piece was able to actually move left
              p_audio->sounds->play(MOVE_SOUND);              
              ghostPiece->moveLeft();
            }
        }       // after a delay, button being held causes piece to move much faster
      else if(p_input->keyDown(SDL_SCANCODE_LEFT) || p_gamepad->isButtonPressed(DPAD_LEFT))
        {
          if (currTime - keyPressStartTime > REPEAT_MOVE_DELAY)
            {
              keyPressStartTime = 0;
              
              if (currPiece->moveLeft())
                {
                  // play move sound effect only if piece was able to actually move left
                  p_audio->sounds->play(MOVE_SOUND);              
                  ghostPiece->moveLeft();
                }
            }
        }

      // handle piece move right events in 2 stages normal, and repeat key hits
      if (p_input->keyHit(SDL_SCANCODE_RIGHT) || p_gamepad->isButtonHit(DPAD_RIGHT))
        {
          keyPressStartTime = currTime;

          if (currPiece->moveRight())
            {
              // play move sound effect only if piece was able to actually move right
              p_audio->sounds->play(MOVE_SOUND);              
              ghostPiece->moveRight();
            }
        }
        
      // after a delay, button being held causes piece to move much faster
      if(p_input->keyDown(SDL_SCANCODE_RIGHT) || p_gamepad->isButtonPressed(DPAD_RIGHT))
        {
          if (currTime - keyPressStartTime > REPEAT_MOVE_DELAY)
            {
              keyPressStartTime = 0;
              
              if (currPiece->moveRight())
                {
                  // play move sound effect only if piece was able to actually move right
                  p_audio->sounds->play(MOVE_SOUND);              
                  ghostPiece->moveRight();
                }
            }
        }

      
      // handle piece rotation event
      if (p_input->keyHit(SDL_SCANCODE_SPACE) || p_gamepad->isButtonHit(A_BUTTON))
        {
          // rotate the playing piece and the ghost piece
          if (currPiece->rotate())
            {
              // play move sound effect
              p_audio->sounds->play(SPIN_SOUND);
      
              // reset the ghost piece to where the current piece is so that rotate doesn't fail
              // from it being on the ground already
              ghostPiece->setColumn(currPiece->getCol());
              ghostPiece->setRow(currPiece->getRow());
              ghostPiece->rotate();
          
              // also rotate the tiles for ghost pieces to draw correctly and reset at 360 degrees
              ghostRotateAngle = (ghostRotateAngle + 90) % 360;
            }
        }

      // handle piece rotation counter-clockwise event
      if (p_input->keyHit(SDL_SCANCODE_BACKSPACE) || p_gamepad->isButtonHit(B_BUTTON))
        {
          // rotate the playing piece and the ghost piece
          if (currPiece->rotateBack())
            {
              // play move sound effect
              p_audio->sounds->play(SPIN_SOUND);
      
              // reset the ghost piece to where the current piece is so that rotate doesn't fail
              // from it being on the ground already
              ghostPiece->setColumn(currPiece->getCol());
              ghostPiece->setRow(currPiece->getRow());
              ghostPiece->rotateBack();
          
              // FIXME rotation of sprites not working 
              //also rotate the tiles for ghost pieces to draw correctly and reset at 360 degrees
              ghostRotateAngle = (ghostRotateAngle - 270) % 360;
            }
        }
      
      // Handle piece swapping event TODO could also make this predicated on clearing a line or tetris
      if (p_input->keyHit(SDL_SCANCODE_LSHIFT) || p_input->keyHit(SDL_SCANCODE_RSHIFT)
          || p_gamepad->isButtonHit(LEFT_SHOULDER) || p_gamepad->isButtonHit(RIGHT_SHOULDER))
        {
          if(canSwap)
            { // check if there's a hold piece in waiting
              if (holdPiece >= 0)
                {
                  // swap the contents of hold piece and current piece
                  currPiece = pieces[holdPiece];
                  ghostPiece = ghostPieces[holdPiece];
                  holdPiece = pieceIDs.front();
                }
              else // there's currently no pieces in the holding area
                {
                  // swap in to the holding area the current piece and get the next from the queue
                  holdPiece = pieceIDs.front();
                  currPiece = pieces[pieceIDs[1]];
                  ghostPiece = ghostPieces[pieceIDs[1]];
                  // add another piece into the queue
                  pieceIDs.pop_front();
                  pieceIDs.push_back(rand() % 7);
                }
              // play swap sound effect
              p_audio->sounds->play(SWAP_SOUND);
          
              // reset the new current piece completely
              currPiece->reset();
              ghostPiece->reset();
              ghostRotateAngle = 0;
              // make sure we only allow a piece swap once per dropped piece 
              canSwap = false;
            }
        }
    }
  // update the ghost piece column each update to make sure it draws to the right spot
  ghostPiece->setColumn(currPiece->getCol());
  ghostPiece->setRow(currPiece->getRow());
  ghostPiece->drop();
} // END UPDATE

 
//TODO get rid of the notion of passing graphics to draw each time (just construct w/)
void RunState::draw(Graphics* graphics)
{
  // draw the background, console, and statistics and landed pieces 
  drawEnvironment();
  playField.draw(graphics);
  
  // draw the pieces in waiting and in hold and active piece
  currPiece->draw(graphics);
  ghostPiece->draw(graphics, ghostRotateAngle);
  
  // draw the hold piece
  if (holdPiece >= 0)
    {
      pipelinePieces[holdPiece]->draw(graphics, 225 + pipelinePieces[holdPiece]->getXOffset()
                                      , 153 + pipelinePieces[holdPiece]->getYOffset());
    }

  //draw the next piece
  pipelinePieces[pieceIDs[1]]->draw(graphics, 715 + pipelinePieces[pieceIDs[1]]->getXOffset()
                                    , 153 + pipelinePieces[pieceIDs[1]]->getYOffset());

  // draw the pipeline pieces
  for (int i = 2; i < 6; i++)
    {
      int xPos = 715 + pipelinePieces[pieceIDs[i]]->getXOffset();
      int yPos = 315 + (i - 2) * 100 + pipelinePieces[pieceIDs[i]]->getYOffset();
      pipelinePieces[pieceIDs[i]]->draw(graphics, xPos, yPos);
    }
} // END DRAW





void RunState::drawEnvironment()
{
  // draw the background after piece to cover up pieces in staging area if adding abilities
  backgroundImg.draw(p_graphics, 0, 0); 

  //draws the console to the center of the screen dimensions
  m_console.draw(p_graphics, (m_screenWidth - 656)/2, 64);

  // Draw the statistics for the current player

  // level & lines
  statsFont.draw(p_graphics, std::to_string(level),215, 473);
  statsFont.draw(p_graphics, std::to_string(totalLines), 215, 537);

  //  time -- alternate approach -- should use boost to format properly (or C++20) or could use ostringstream
  seconds = timer.getTicks() / 1000;
  std::sprintf(timerText, "%01d:%02d:%02d",
               seconds / 3600,
               (seconds / 60) % 60,
               seconds % 60);
  statsFont.draw(p_graphics, timerText, 215, 601);

  //score
  score = totalLines * 50 + tetrises * 100;
  statsFont.draw(p_graphics, std::to_string(score), 215, 665);
} 


void RunState::outputStats()
{
  // remove previous stats from text overlay file
  stats.clearText();

  // format our game data
  std::stringstream statString;
  statString << "Time: " << timerText << std::endl;
  statString << "Level: " << level << std::endl;
  statString << "Cleared Lines: " << totalLines << std::endl;
  statString << "Tetrises: " << tetrises <<std::endl;
  statString << "--------------------------" << std::endl;
  statString << "Total Score: " << score;

  // finally add the data to the stats file 
  stats.addLine(statString.str());
}


void RunState::reset()
{
  // set the starting level
  level = 1;
  tetrises = 0;
  totalLines = 0;
  dropTime = 800;
  
  // start the timer
  timer.start();
  lastDropTime = timer.getTicks();
  manueverStartTime = timer.getTicks();

  // pick a new pieces and place them in the pipeline queue
  pieceIDs.clear();
  for (int i = 0; i < 6; i++)
    {
      pieceIDs.push_back(rand() % 7);
    }
  currPiece = pieces[pieceIDs.front()];
  ghostPiece = ghostPieces[pieceIDs.front()];
  ghostRotateAngle = 0;
  
  // clear all tiles from playfield and rotate current piece to starting position
  playField.setAllTiles(0);
  currPiece->reset();
  ghostPiece->reset();
  holdPiece = -1;
  canSwap = true;
}


void RunState::unpauseTimer()
{
  timer.unPause();
}
