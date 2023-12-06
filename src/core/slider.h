//
//  slider.h
//
#ifndef SLIDER_H_
#define SLIDER_H_

#include "image.h"
#include "input.h"
#include "gamepad.h"


class Slider
{
private:
  // shared resources
  Gamepad* p_gamepad;
  Input* p_input;
  
  Image m_sliderImg;
  Image m_sliderHighlight;
  Image m_knobImg;
  // knob highlighting image should be same dimensions as the knob image 
  Image m_knobHighlightImg;
  Image m_barImg;

  int m_divisions;
  //  int m_knobPosition; // X offset from off (leftmost) position
  int m_maximum;
  int highlightOffsetX;
  int highlightOffsetY;
  int barOffsetX;
  int barOffsetY;
  int knobOffsetX;
  int knobOffsetY;
  bool knobIsActivated;
  bool highlightSlider;
  
  void update(int xPos, int yPos);
  
public:
  Slider();
  // knobPosition == -1 will center the knob in the slider
  bool init(Graphics* gfx, Gamepad* gamepad, Input* input, std::string sliderImgFile
            , std::string knobImgFile, std::string barImgFile
            , std::string highlightImgFile, std::string knobHighlightFile, int maximum
            , int divisions = 0, int knobPosition = -1);

  void setHighlight(bool isHighlighted) { highlightSlider = isHighlighted; }
  void activate() { knobIsActivated = true; }
  void deactivate() { knobIsActivated = false; }
  bool isActivated() { return knobIsActivated; }
  int draw(Graphics* gfx, int xPos, int yPos);
  void free();
};
#endif
