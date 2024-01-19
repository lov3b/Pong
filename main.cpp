#include "Game.h"


int main() {
    Game game(SDL_Point{1000, 600});

    return game.loop();
}
