cmake_minimum_required(VERSION 3.27)
project(Pong)

set(CMAKE_CXX_STANDARD 17)

# Base SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# SDL2 Gfx
find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx SDL2_gfxd)
if (NOT SDL2_GFX_LIBRARY)
    message(FATAL_ERROR "SDL2_gfx not found")
endif ()

find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf SDL2_TTF SDL2TTF)
if (NOT SDL2_TTF_LIBRARY)
    message(FATAL_ERROR "SDL2_TTF not found")
endif ()



# Define the executable target before linking libraries
add_executable(Pong main.cpp
        SdlWrapper.h
        Game.h
        VisibleObjects/Ball.h
        utils.h
        Vec2d/Vec2d.h
        Vec2d/Bump.h
        VisibleObjects/PlayerPaddle.h
        VisibleObjects/Side.h
        VisibleObjects/Score.h)

# Now link the libraries to the target
target_link_libraries(Pong ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARY} ${SDL2_TTF_LIBRARY})