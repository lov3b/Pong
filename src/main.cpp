#include "Game.h"
#include <SDL.h>


extern "C" int main(int _argc, char *_argv[]) {
    Game game(SDL_Point{1000, 600});

    return game.loop();
}
