// SPDX-License-Identifier: BSD-2-Clause

#include "Game.h"

Game::Game(SDL_Point screenSize) : SdlWrapper("Pong", screenSize, 60),
                                   leftPaddle(new PlayerPaddle(&this->screenSize, Side::LEFT)),
                                   rightPaddle(new PlayerPaddle(&this->screenSize, Side::RIGHT)),
                                   score(new Score(&this->screenSize, 5)),
                                   ball(new Ball(&this->screenSize, leftPaddle, rightPaddle, score)),
                                   startScreen(
                                           new ScrollOptionScreen(
                                                   "Welcome to Pong!\nUse arrowkeys to adjust max score.\nFinish with enter.",
                                                   &this->screenSize, 4, score->maxScore())), endScreen(nullptr),
                                   gameState(GameState::START_SCREEN) {
}

Game::~Game() {
    delete ball;
    delete leftPaddle;
    delete rightPaddle;
    delete score;
}

void Game::draw(SDL_Renderer *const renderer) {
    // Background
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 0);
    SDL_RenderClear(renderer);

    switch (gameState) {
        case GameState::START_SCREEN:
            startScreen->draw(renderer);
            break;
        case GameState::GAME:
            ball->draw(renderer);
            score->draw(renderer);
            leftPaddle->draw(renderer);
            rightPaddle->draw(renderer);
            break;
        case GameState::END_SCREEN:
            endScreen->draw(renderer);
            break;
    }

    SDL_RenderPresent(renderer);
}

void Game::update() {
    switch (gameState) {
        case GameState::START_SCREEN:
            startScreen->update();
            if (startScreen->isDone()) {
                gameState = GameState::GAME;
                delete startScreen; // We will never get back to this state
            }
            break;
        case GameState::GAME:
            ball->update();
            leftPaddle->update();
            rightPaddle->update();
            score->update();

            if (score->sideWon().has_value()) {
                const char *player = score->sideWon().value() == Side::LEFT ? "left" : "right";
                std::stringstream ss;
                ss << "The " << player << " player won with " << std::to_string(score->leftScore) << " - "
                   << std::to_string(score->rightScore) << "\nWould you like to play again?"
                   << "\nIf so, press any button...";
                score->resetScore();
                endScreen = new OptionScreen(ss.str(), &screenSize, 4);
                gameState = GameState::END_SCREEN;
            }
            break;
        case GameState::END_SCREEN:
            endScreen->update();
            if (endScreen->isDone()) {
                gameState = GameState::GAME;
                delete endScreen; // The text will not be the same if we get back here. We might as well free it.
            }
            break;
    }
}

bool Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;

        switch (gameState) {
            case GameState::START_SCREEN:
                startScreen->handleEvent(event);
                break;
            case GameState::GAME:
                handleGameEvent(event);
                break;
            case GameState::END_SCREEN:
                if (event.type == SDL_KEYDOWN && !endScreen->hasStartedCounting())
                    endScreen->startCountDown();
                break;
        }
    }

    return true;
}

void Game::handleGameEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                leftPaddle->startMoving(true);
                break;
            case SDLK_s:
                leftPaddle->startMoving(false);
                break;

            case SDLK_UP:
                rightPaddle->startMoving(true);
                break;
            case SDLK_DOWN:
                rightPaddle->startMoving(false);
                break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                leftPaddle->stopMoving(true);
                break;
            case SDLK_s:
                leftPaddle->stopMoving(false);
                break;

            case SDLK_UP:
                rightPaddle->stopMoving(true);
                break;
            case SDLK_DOWN:
                rightPaddle->stopMoving(false);
                break;

        }

    }
}