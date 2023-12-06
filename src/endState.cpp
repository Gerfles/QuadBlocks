//
// endState.cpp
//
// game over screen
#include "endState.h"
#include "SDL2/SDL_scancode.h"
#include "outlineFont.h"
#include "textOverlay.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits.h>
#include "runState.h"

bool EndState::init(Graphics* gfx, Input* in, Gamepad* gamepad, StateManager* manager
                    , TextOverlay* stats, RunState* prevState)
{
  // grab keyboard and gamepad input
  p_graphics = gfx;
  p_input = in;
  p_gamepad = gamepad;

  // previous state used to render graphics to the screen
  m_prevState = prevState;
  p_stats = stats;
  
  // set the manager of this State
  setManager(manager);

  if (!background.load(gfx, "graphics/gameover.png"))
    return false;

  if (!p_stats->load(gfx, "text/stats.ovl"))
    return false;

  if (!nameEntry.load(p_graphics, p_input, p_gamepad, "text/pixel.ttf", 42, 250, 250, 60))
    return false;

  // create an image to instruct for name input
  if(!font.load("text/pixel.ttf", 42))
    return false;
  
 font.getFontImage(p_graphics, &promptForName, "Enter Your Name:", 250, 250, 60);

  return true;

} // END init


void EndState::reset(int score)
{
  m_score = score;
  p_stats->load(p_graphics, "text/stats.ovl");
  m_getPlayerName = false;
  displayLeaderboard = false;

  // load current high scores to compare to and immediately write them back to file in case user quits early
  hiScores.clear();
  loadLeaderboard();
  updateLeaderboard();

  // check if leaderboard has any empty slots
  if (hiScores.size() < 13)
    {// make sure to take in current user score to fill the empty space
      m_getPlayerName = true;
    }

  // enable text input if we're going to be getting the player's name
  if (m_getPlayerName == true)
    {
      nameEntry.enableTextInput();
    }
} // END reset


void EndState::free()
{
  background.free();
  nameEntry.free();
  font.free();
}


void EndState::update()
{
  // user wants to continue playing 
  if (p_input->keyHit(SDL_SCANCODE_RETURN) || p_gamepad->isButtonHit(START_BUTTON))
    {
      // only advance if we are not currently entering in user name
      if (!m_getPlayerName)
        {
          m_prevState->reset();
          m_prevState->unpauseTimer();
          p_stats->load(p_graphics, "text/stats.ovl");
          getManager()->popState();
        }
    }

  // quit the game and go back to main menu
  if(p_input->keyHit(SDL_SCANCODE_ESCAPE) || p_gamepad->isButtonHit(BACK_BUTTON))
    {
      getManager()->popState();
      getManager()->popState();
    }

  // alternate between displaying personal stats and leaderboard
  if ((p_input->keyHit(SDL_SCANCODE_SPACE) || p_gamepad->isButtonHit(A_BUTTON)) && !m_getPlayerName)
    {       
      (displayLeaderboard) ? displayLeaderboard = false : displayLeaderboard = true;
      if (displayLeaderboard)
        {
          p_stats->load(p_graphics, "text/hiScores.ovl");
        }
      else
        {
          p_stats->load(p_graphics, "text/stats.ovl");
        }
    }
}


void EndState::draw(Graphics *gfx)
{
  // keep previous block placement on screen
  m_prevState->draw(gfx);

  // draw background
  background.draw(gfx, 0, 0);

  // draw the stats to the screen
  p_stats->draw();

  // draw text strings
  if (m_getPlayerName)
    {
      // draw the name entry prompt until user finished (returns false when return or escape key hit)
      promptForName.draw(p_graphics, 250, 450);
      if (!nameEntry.draw(1024/2, 510))
        {
          // reload current high scores into map but so we will delete existing lines in file
          hiScores.clear();
          loadLeaderboard();
          // add current user name and score into high scores map and write into leaderboard with others
          hiScores.insert(std::pair<int, std::string>(m_score, nameEntry.getText() + " "));
          updateLeaderboard();

          // make sure we no longer draw text input
          m_getPlayerName = false;

          // refresh high scores to make sure our entered name shows up
          p_stats->load(p_graphics, "text/hiScores.ovl");
        }
    }
}


void EndState::loadLeaderboard()
{
  // open the leaderboard file
  std::fstream leaderboard("text/hiScores.ovl");

  // make sure the file opened properly
  if (!leaderboard.is_open())
    {
      std::cerr << "Could not open hi_scores.ovl\n";
      leaderboard.clear();
      exit(EXIT_FAILURE);
    }

  // store  all the data used by Textoverlay to format text
  std::vector<std::string> buffer(13);
  for (int i = 0; i < 13; i++)
    {
      getline(leaderboard, buffer.at(i), '\n');
    }

  // temps to store contents of leaderboard file
  char scoreText[31];
  char name[31];

  // read all the names and score in from leaderboard
  while (leaderboard.getline(name, 30, '-'))
    {
      leaderboard.getline(scoreText, 30, '\n');
      if (leaderboard.eof())
        {
          break;
        }
      // convert score chars to numbers to compare them to our score later
      int score = std::atoi(scoreText);
      // save the remaining scores and names into a map for ordering
      hiScores.insert(std::pair<int, std::string>(score, std::string(name)));
      // if current player's score is greater than one of the scores in the leaderboard
      if (m_score > score)
        { // set the flag to allow to user name entry
          m_getPlayerName = true;
        }
    }

  // close the file and reopen to erase the current file and replace only the formatting data
  leaderboard.close();
  leaderboard.open("text/hiScores.ovl", std::ios::out | std::ios::trunc);

  // put the formatting details back into the file, thus deleting any previous text lines
  for (std::string line : buffer)
    {
      leaderboard << line << std::endl;
    }

  // close the file to update
  leaderboard.close();
}


void EndState::updateLeaderboard()
{
  // open the leaderboard file for appending the high scores to the end
  std::fstream leaderboard;
  leaderboard.open("text/hiScores.ovl", std::ios::out | std::ios::app);
    
  int scoreEntry = 1;

  // write only the top 13 scores back into the leaderboard
  for (auto iter = hiScores.rbegin(); iter != hiScores.rend(); ++iter)
    {
      if (scoreEntry > 13)
        {
          break;
        }
      
      leaderboard << iter->second << "- " << iter->first << "    " << std::endl;
      scoreEntry++;
    }
  // close the file to update
  leaderboard.close();
}
