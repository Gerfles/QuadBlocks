//
// textOverlay.cpp
//
#include "textOverlay.h"
#include "SDL2/SDL_timer.h"
#include "graphics.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>


bool TextOverlay::load(Graphics* gfx, const std::string filename)
{
  invert = 1;
  graphics_p = gfx;
  m_filename = filename;

  std::ifstream in(m_filename);

   if (!in.good())
    {
      SDL_Log("TextOverlay class unable to open file: %s\n", filename.c_str());
      return false;
    }

  // Get all the specs for the file
  std::string buffer;

  // discard the word 'font_size' 
  getline(in, buffer, ' ');
  // get the trailing data (font size) and save 
  getline(in, buffer, '\n');
  fontSize = atoi(buffer.c_str());

  // get the font file name
  getline(in, buffer, ' ');
  getline(in, buffer, '\n');

  // attempt to open the font 
  if (!font.load(buffer, fontSize))
    {
      SDL_Log("TextOverlay class unable to load font: %s\n", buffer.c_str());
      return false;
    }

  // get the screen dimension offsets 
  getline(in, buffer, ' ');
  getline(in, buffer, ' ');
  offsetX = atoi(buffer.c_str());
  getline(in, buffer, '\n');
  offsetY = atoi(buffer.c_str());

  // get the size of the display box
  getline(in, buffer, ' ');
  getline(in, buffer, ' ');
  width = atoi(buffer.c_str());
  getline(in, buffer, '\n');
  height = atoi(buffer.c_str());

  // get the color of text
  getline(in, buffer, ' ');
  getline(in, buffer, ' ');
  r = atoi(buffer.c_str());
  getline(in, buffer, ' ');
  g = atoi(buffer.c_str());
  getline(in, buffer, '\n');
  b = atoi(buffer.c_str());

  // get the spacing
  getline(in, buffer, ' ');
  getline(in, buffer, '\n');
  spacing = atoi(buffer.c_str());

  // get the method for displaying strings
  getline(in, buffer, ' ');
  getline(in, buffer, '\n');
  if (buffer == "BULLET")
    mode = TextOverlay::BULLET;
  else if (buffer == "EMPHASIS")
    mode = TextOverlay::EMPHASIS;
  else
    mode = TextOverlay::SCROLL;

  // get the speed
  getline(in, buffer, ' ');
  getline(in, buffer, '\n');
  speed = atoi(buffer.c_str());

  // get the linger time
  getline(in, buffer, ' ');
  getline(in, buffer, '\n');
  linger = atoi(buffer.c_str());

  // get the alignment
  getline(in, buffer, ' ');
  getline(in, buffer, '\n');
  if (buffer == "LEFT")
    alignment = TextOverlay::LEFT;
  else if (buffer == "RIGHT")
    alignment = TextOverlay::RIGHT;
  else
    alignment = TextOverlay::CENTER;
  
  // determine if inverted
  getline(in, buffer, ' ');
  getline(in, buffer, '\n');
  if (buffer == "TRUE")
    invert = -1;
    
  // reset lines vector to clear any previous lines from a previous load
  lines.clear();
  // get the rest of the text and store into string vector
  while (std::getline(in, buffer, '\n'))
    {
      lines.push_back(buffer);
    }
  numLines = lines.size();
  
  // close strings file
  in.close();

  reset();
  return true;
}


int TextOverlay::draw()
{
  // start the timer if need be
  if (!timer.hasStarted())
    timer.start();
  
  // check if all text has been displayed before redrawing
  if (finished == true && linger > 0)
    {
      // wait until linger time has elapsed before signaling for quit
      if (timer.getTicks() > (Uint32)linger)
        return -1;
    }

  // draw the lines of text based on the file-specified method
  switch (mode)
    {
    case SCROLL:
      {
        drawScroll();
        break;
      }
    case BULLET:
      {
        drawBullet();;
        break;
      }
    case EMPHASIS:
      {
        drawEmphasis();
        break;
      }
    default:
      break;
    }
  return numDrawLines;
}

 
void TextOverlay::drawBullet()
{
  if (speed > 0)
    {
      numDrawLines = (timer.getTicks() / speed);
    }

  for (int index, i = 0; i < numDrawLines; i++)
    {
      // check for inversion, make index start at end of vector if so
      if (invert < 0)
        index = numLines - i - 1;
      else // no inversion
        index = i;

      // skip any blank lines
      if (lines[index] == "" && numDrawLines < numLines)
        {
          numDrawLines++;
        }
      // check to see if all the necessary lines have been drawn
      else if (numDrawLines >= numLines)
        {
          numDrawLines = numLines;
          if (finished != true)
            { 
              finished = true;
              // make sure to linger starting after all lines drawn
              if (linger > 0)
                linger += timer.getTicks();
            }
        }
      drawLine(index, index * spacing);
    }
}                      


// only draw one line at a time and keep on screen until enough time (speed, in ms) has passed 
void TextOverlay::drawEmphasis()
{
  int numDrawLines = 0;

  if (speed > 0)
    {
      numDrawLines = timer.getTicks() / speed;
      numDrawLines += skipLines;
    }

  if (numDrawLines >= numLines)
    {
      numDrawLines = numLines - 1;
    
      if (finished != true)
        {
          finished = true;
          // make sure to linger an additional amount of time
          if (linger > 0)
            linger += timer.getTicks();
        }
    }

  if (lines[numDrawLines] == "" && numDrawLines < numLines - 1)
    {
      numDrawLines++;
      skipLines++;
    }
  drawLine(numDrawLines, 0);
}


void TextOverlay::drawScroll()
{
  
  Uint32 elapsedTime = timer.getTicks() - prevTime;
  prevTime = timer.getTicks();

  // use float for top since step size may be less than 1, add 0.5 to round for smoother animation
  top -= invert * ((speed * elapsedTime) / 1000.F) + 0.5;
  int yPos;
  
  for (int i = 0; i < numLines; i++)
    {
      yPos = top + invert * i * spacing;
      
      // only draw the lines within the screen
      if (lines[i] != "" && yPos < height && yPos + spacing > 0)
        {
          drawLine(i, yPos);
          numDrawLines = i;
        }
    }

  //set exit flag based on the position of the last line
  if ((invert > 0 && yPos + spacing < 0)
      || (invert < 0 && yPos + spacing > height))
    {
     if (finished != true)
        {
          finished= true;
          if (linger > 0)
            linger += timer.getTicks();
        }
    }
}


void TextOverlay::drawLine(int index, int yPosition)
{
  int xPos;
  
  if (lines[index] != "")
        {
          switch (alignment)
            {
            case CENTER:
              {
                // instead of just drawing text -> use 2 step method to get width of text!
                font.createFontImage(graphics_p, lines[index], r, g, b, OutlineFont::BLENDED);
                xPos = (width - font.getWidth()) / 2;
                font.draw(graphics_p, xPos + offsetX, yPosition + offsetY);
                break;
              }
            case LEFT:
              {
                font.draw(graphics_p, lines[index], offsetX, yPosition + offsetY
                          , r, g, b, OutlineFont::BLENDED);
                break;
              }
            case RIGHT:
              {
                font.createFontImage(graphics_p, lines[index], r, g, b, OutlineFont::BLENDED);
                xPos = width - font.getWidth();
                font.draw(graphics_p, xPos + offsetX, yPosition + offsetY);
                break;
              }
            default:
              break;
            }
        }
}


void TextOverlay::clearText()
{
  // open file and check that it is indeed open
  std::fstream out(m_filename);
  if (!out.is_open())
    {
      std::cerr << "Could not open file: " << m_filename << std::endl;
      out.clear();
      exit(EXIT_FAILURE);
    }

  //  save the text we need into a vector
  std::vector<std::string> buffer(TEXT_START_LINE);
  for (int i = 0; i < TEXT_START_LINE; i++)
    {
      getline(out, buffer.at(i), '\n');
    }
  
  // now close the file and re-open to delete contents
  out.close();
  out.open(m_filename, std::ios::out | std::ios::trunc);

  if (!out.is_open())
    {
      std::cerr << "Could not open file: " << m_filename << std::endl;
      out.clear();
      exit(EXIT_FAILURE);
    }

  // put the formatting details back into the file, thus deleting previous text lines
  for (std::string line : buffer)
    {
      out << line << std::endl;
    }

  // close file to refresh
  //  out.flush();
  out.close();
}


// TODO consider allowing a vector of strings to be add all at once
void TextOverlay::addLine(std::string line)
{
  // open file and check that it is good 
  std::fstream out;
  out.open(m_filename, std::fstream::app);
  
  if (!out.is_open())
    {
      std::cerr << "Could not open file: " << m_filename << std::endl;
      out.clear();
      exit(EXIT_FAILURE);
    }
  // append the new line onto the file
  out << line << std::endl;

  // close file to update
  // out.flush();
  out.close();
  
} // END addLine(std::string line)


void TextOverlay::reset()
{
  finished = false;
  prevTime = 0;
  numDrawLines = 0;
  
  // set top of lines to the bottom of screen or 0 if invert
  if (invert < 0)
      top = 0 - fontSize;
  else 
      top = height;
  
  timer.stop();
}

void TextOverlay::free()
{
  font.free();
  finished = false;
  timer.stop();
}
