//
// Created by love on 2024-01-18.
//

#pragma

#include "SdlWrapper.h"
#include "SDL.h"
#include "VisibleObjects/Ball.h"
#include "VisibleObjects/PlayerPaddle.h"
#include "VisibleObjects/Score.h"

enum class GameState {
    START_SCREEN, GAME, END_SCREEN
};

class Game : public SdlWrapper {
private:
    Ball *ball;
    Score *score;
    PlayerPaddle *leftPaddle, *rightPaddle;

protected:
    GameState gameState;

public:
    explicit Game(SDL_Point screenSize) : SdlWrapper("Pong", screenSize, 60) {
        leftPaddle = new PlayerPaddle(&this->screenSize, Side::LEFT);
        rightPaddle = new PlayerPaddle(&this->screenSize, Side::RIGHT);

        auto func = [this](Side side) {
            const char *player = side == Side::LEFT ? "one" : "two";
            std::cout << "Player " << player << " won" << std::endl;
            this->running = false;
        };
        score = new Score(5, &this->screenSize, func);
        ball = new Ball(&this->screenSize, leftPaddle, rightPaddle, score);
        gameState = GameState::START_SCREEN;
    }

    ~Game() override {
        delete ball;
        delete leftPaddle;
        delete rightPaddle;
        delete score;
    }

    void draw(SDL_Renderer *renderer) override {
        // Background
        SDL_SetRenderDrawColor(renderer, 128, 0, 128, 0);
        SDL_RenderClear(renderer);


        switch (gameState) {
            case GameState::START_SCREEN:
                break;
            case GameState::GAME:
                ball->draw(renderer);
                score->draw(renderer);
                leftPaddle->draw(renderer);
                rightPaddle->draw(renderer);
                break;
            case GameState::END_SCREEN:
                break;
        }

        SDL_RenderPresent(renderer);
    }

    bool update() override {
        switch (gameState) {
            case GameState::START_SCREEN:
                break;
            case GameState::GAME:
                ball->update();
                leftPaddle->update();
                rightPaddle->update();
                score->update();
                break;
            case GameState::END_SCREEN:
                break;
        }

        return true;
    }

    bool handleEvents() override {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                return false;

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
        return true;
    }
};