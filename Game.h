//
// Created by love on 2024-01-18.
//

#pragma

#include "SdlWrapper.h"
#include "SDL.h"
#include "VisibleObjects/Ball.h"
#include "VisibleObjects/PlayerPaddle.h"


class Game : public SdlWrapper {
private:
    Ball *ball;

    PlayerPaddle *leftPaddle, *rightPaddle;

public:
    explicit Game(SDL_Point screenSize) : SdlWrapper("Pong", screenSize, 60) {
        leftPaddle = new PlayerPaddle(&this->screenSize, Side::LEFT);
        rightPaddle = new PlayerPaddle(&this->screenSize, Side::RIGHT);
        ball = new Ball(&this->screenSize, leftPaddle, rightPaddle);
    }

    ~Game() override {
        delete ball;
        delete leftPaddle;
        delete rightPaddle;
    }

    void draw(SDL_Renderer *renderer) override {
        // Background
        SDL_SetRenderDrawColor(renderer, 128, 0, 128, 0);
        SDL_RenderClear(renderer);

        ball->draw(renderer);
        leftPaddle->draw(renderer);
        rightPaddle->draw(renderer);
        SDL_RenderPresent(renderer);
    }

    bool update() override {
        ball->update();
        leftPaddle->update();
        rightPaddle->update();

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