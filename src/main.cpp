//
// main.cpp
//
// demo to show elements of scene management
// compile with:
// clang++ -Wall -Wextra  -std=c++11 -F/Library/Frameworks -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer core/game.cpp core/audio.cpp core/sound.cpp core/music.cpp core/graphics.cpp core/image.cpp core/stateManager.cpp core/input.cpp core/outlineFont.cpp core/rasterFont.cpp core/gameState.cpp core/scene.cpp core/rectangle.cpp core/button.cpp core/sceneNode.cpp core/sprite.cpp main.cpp tetris.cpp runState.cpp mainMenuState.cpp creditsState.cpp 

#include "quadBlocks.h"


int main(int argc, char* argv[])
{
  Tetris game;

  if (!game.init())
    {
      game.free();
      return -1;
    }

  game.run();
    
  return 0;
}

