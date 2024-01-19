//
// Created by love on 2024-01-18.
//
#pragma once

#include "../utils.h"
#include "../Vec2d/Vec2d.h"
#include "PlayerPaddle.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include "optional"

class Ball {
private:
    static const uint8_t RADIUS = 15;
    const SDL_Point *screen;
    Sint16 x, y;
    Vec2d *vec2d;
    static const uint32_t color = 0xCD5C5CFF;
    const PlayerPaddle *leftPaddle, *rightPaddle;

public:
    explicit Ball(const SDL_Point *screen, const PlayerPaddle *leftPaddle, const PlayerPaddle *rightPaddle) {
        this->screen = screen;
        this->x = screen->x / 2;
        this->y = screen->y / 2;
        this->vec2d = new Vec2d(2.5);
        this->leftPaddle = leftPaddle;
        this->rightPaddle = rightPaddle;
    }

    void draw(SDL_Renderer *renderer) const {
        filledCircleColor(renderer, x, y, RADIUS, color);
    }

    void update() {
        std::cout << "Ball x: " << x << ", y: " << y << std::endl;
        std::optional<Side> paddleSide = collidedPaddle();
        bool screenEdgeVertical = collidedScreenEdgeVertical();
        if (screenEdgeVertical && paddleSide.has_value()) {
            vec2d->bump(BumpType::BOTH, PaddleDirection::NONE);
        } else if (screenEdgeVertical) {
            vec2d->bump(BumpType::WALL, PaddleDirection::NONE);
        } else if (collidedScreenEdgeHorizontal()) {
            std::cout << "Player won" << std::endl;
            exit(1);
        }
        if (paddleSide.has_value()) {
            const PlayerPaddle *paddle = paddleSide.value() == Side::LEFT ? leftPaddle : rightPaddle;
            vec2d->bump(BumpType::PADDLE, paddle->getPaddleDirection());
        }
        vec2d->applyVector(&x, &y);
    }

private:
    bool collidedScreenEdgeVertical() {
        return y - RADIUS <= 0 || y + RADIUS >= screen->y;
    }

    bool collidedScreenEdgeHorizontal() {
        return x + RADIUS >= screen->x || x - RADIUS <= 0;
    }

    std::optional<Side> collidedPaddle() {
        // Check collision with right paddle
        if (x + RADIUS >= rightPaddle->x &&
            y >= rightPaddle->y &&
            y <= rightPaddle->y + rightPaddle->h) {
            return Side::RIGHT;
        }
            // Check collision with left paddle
        else if (x - RADIUS <= leftPaddle->x + leftPaddle->w &&
                 y >= leftPaddle->y &&
                 y <= leftPaddle->y + leftPaddle->h) {
            return Side::LEFT;
        }
        return std::nullopt;
    }


};


