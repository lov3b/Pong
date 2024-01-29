// SPDX-License-Identifier: BSD-2-Clause

#pragma

#include "SdlWrapper.h"
#include "SDL.h"
#include "VisibleObjects/Ball.h"
#include "VisibleObjects/PlayerPaddle.h"
#include "text/Score.h"
#include "text/TextScreen.h"
#include "text/OptionScreen.h"
#include "text/Score.h"
#include "text/ScrollOptionScreen.h"

enum class GameState {
    START_SCREEN, GAME, END_SCREEN
};

class Game : public SdlWrapper {
private:
    Score *score;
    PlayerPaddle *leftPaddle, *rightPaddle;
    OptionScreen *endScreen;
    ScrollOptionScreen *startScreen;
    Ball *ball;

protected:
    GameState gameState;

public:
    explicit Game(SDL_Point screenSize);

    ~Game() override;

    void draw(SDL_Renderer *renderer) override;

    void update() override;

    bool handleEvents() override;

    void handleGameEvent(SDL_Event &event);
};