cmake_minimum_required(VERSION 3.25)
project(Pong)

set(CMAKE_CXX_STANDARD 17)

# Option for enabling optimizations
option(ENABLE_OPTIMIZATIONS "Enable compiler optimizations" OFF)

# Base SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# SDL2 Gfx
find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx SDL2_gfxd libSDL2_gfx)
if (NOT SDL2_GFX_LIBRARY)
    message(FATAL_ERROR "SDL2_gfx not found")
endif ()

find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf SDL2_TTF SDL2TTF)
if (NOT SDL2_TTF_LIBRARY)
    message(FATAL_ERROR "SDL2_TTF not found")
endif ()


# Define the executable target before linking libraries
add_executable(Pong src/main.cpp
        src/SdlWrapper.h
        src/SdlWrapper.cpp
        src/Game.h
        src/Game.cpp
        src/VisibleObjects/Ball.h
        src/VisibleObjects/Ball.cpp
        src/Vec2d/Vec2d.h
        src/Vec2d/Vec2d.cpp
        src/Vec2d/Bump.h
        src/VisibleObjects/PlayerPaddle.h
        src/VisibleObjects/PlayerPaddle.cpp
        src/VisibleObjects/Side.h
        src/text/TextScreen.h
        src/text/TextScreen.cpp
        src/defaultfont.h
        src/defaultfont.cpp
        src/icon.h
        src/icon.cpp
        src/text/OptionScreen.h
        src/text/OptionScreen.cpp
        src/text/Score.h
        src/text/Score.cpp
)

# Now link the libraries to the target
target_link_libraries(Pong ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARY} ${SDL2_TTF_LIBRARY})

# Set compiler optimization flags
if (ENABLE_OPTIMIZATIONS)
    message(STATUS "Optimizations are enabled")
    set_target_properties(Pong PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(Pong PROPERTIES LINK_SEARCH_END_STATIC 1)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

    if (MSVC)
        target_compile_options(Pong PRIVATE /O2)
    else ()
        target_compile_options(Pong PRIVATE -O3)
    endif ()
endif ()
