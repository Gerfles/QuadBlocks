cmake_minimum_required(VERSION 3.10)

#set the project name
project(quadblocks VERSION 1.0)

#_____________________________________________________________________
#configure file to version number into source code
#----------------------------------------------------------------------

#specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

#configuration types
# SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
# IF(DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "3.1")
#   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
# ENDIF()
# SET(CMAKE_DEBUG_POSTFIX "d")


# Add compiler errors/warnings flags
# target_compile_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4 /WX>)
# target_compile_options(${PROJECT_NAME} PRIVATE $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

# add the core (engine) library
add_subdirectory("${PROJECT_SOURCE_DIR}/src/core")

# needed by CMake to find the SDL2 headers on OSX
include_directories(/usr/local/include)

# Add all c++ source files under the src directory
set(SOURCE_FILES src/main.cpp src/creditsState.cpp src/optionState.cpp
  src/mainMenuState.cpp src/pauseState.cpp src/runState.cpp src/quadBlocks.cpp src/pieces.cpp src/endState.cpp src/HUD.cpp)
# shouldn't do it this way because make and other programs won't be able to refactor
# file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# add binary tree directory to paths to search for includes
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/src/core")

# Add all headers files under the include directory

#comment out later to see if necessary
#target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/core")

#_____________________________________________________________________
# find all the SDL2 libraries
#---------------------------------------------------------------------
# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add SDL2 library
find_package(SDL2 REQUIRED)

# Add SDL2_image library
find_package(SDL2_image REQUIRED)

# Add SDL2_ttf library
find_package(SDL2_ttf REQUIRED)

# Add SDL2_mixer library
find_package(SDL2_mixer REQUIRED)

# Add SDL2_net library
#find_package(SDL2_net REQUIRED)

# Add SDL2_gfx library
#find_package(SDL2_gfx REQUIRED)
#______________________________________________________________________

# add header files
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} Core)

# add libraries, including from our game engine
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} Core)# "${PROJECT_SOURCE_DIR}/src/core")

# Copy assets to build folder
file(COPY graphics DESTINATION ${CMAKE_BINARY_DIR})
file(COPY text DESTINATION ${CMAKE_BINARY_DIR})
file(COPY audio DESTINATION ${CMAKE_BINARY_DIR})
file(COPY maps DESTINATION ${CMAKE_BINARY_DIR})

