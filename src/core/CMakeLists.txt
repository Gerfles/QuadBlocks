##
##

set(CORE_SOURCES audio.cpp button.cpp factory.cpp game.cpp gameMath.cpp
  gameState.cpp graphics.cpp image.cpp input.cpp map.cpp mapNode.cpp
  music.cpp outlineFont.cpp rasterFont.cpp rectangle.cpp scene.cpp
  sceneNode.cpp sounds.cpp sprite.cpp stateManager.cpp grid.cpp timer.cpp textOverlay.cpp parallax.cpp gamepad.cpp textPrompt.cpp slider.cpp)

# needed by CMake to find the SDL2 headers on OSX
include_directories(/usr/local/include)

add_library(Core ${CORE_SOURCES}) 





