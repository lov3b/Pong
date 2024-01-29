//
// Created by love on 2024-01-18.
//
#pragma once

#include "../Vec2d/Vec2d.h"
#include "PlayerPaddle.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include "optional"
#include "../text/Score.h"
#include "../text/Score.h"

class Ball {
private:
    static const uint8_t RADIUS = 15;
    const SDL_Point *const screen;
    Sint16 x, y;
    Vec2d *vec2d;
    static const uint32_t color = 0xCD5C5CFF;
    const PlayerPaddle *const leftPaddle, *const rightPaddle;
    Score *const score;

public:
    explicit Ball(const SDL_Point *screen, const PlayerPaddle *leftPaddle, const PlayerPaddle *rightPaddle,
                  Score *score) : score(score), screen(screen), leftPaddle(leftPaddle), rightPaddle(rightPaddle),
                                  x(screen->x / 2), y(screen->y / 2), vec2d(new Vec2d(6)) {
    }

    void resetPosition() {
        this->x = screen->x / 2;
        this->y = screen->y / 2;

        delete vec2d;
        vec2d = new Vec2d(6);
    }

    void draw(SDL_Renderer *renderer) const {
        filledCircleColor(renderer, x, y, RADIUS, color);
    }

    void update() {
        std::optional<Side> paddleSide = collidedPaddle();
        bool screenEdgeVertical = collidedScreenEdgeVertical();
        std::optional<Side> scoreSide = collidedScreenEdgeHorizontal();

        if (screenEdgeVertical && paddleSide.has_value()) {
            vec2d->bump(BumpType::BOTH, PaddleDirection::NONE);
        } else if (screenEdgeVertical) {
            vec2d->bump(BumpType::WALL, PaddleDirection::NONE);
        } else if (scoreSide.has_value()) {
            // Invert side
            score->incrementScore(scoreSide.value() == Side::LEFT ? Side::RIGHT : Side::LEFT);
            resetPosition();
        }
        if (paddleSide.has_value()) {
            const PlayerPaddle *paddle = paddleSide.value() == Side::LEFT ? leftPaddle : rightPaddle;
            vec2d->bump(BumpType::PADDLE, paddle->getPaddleDirection());
        }
        vec2d->applyVector(&x, &y);
    }

private:
    [[nodiscard]]bool collidedScreenEdgeVertical() const {
        return y - RADIUS <= 0 || y + RADIUS >= screen->y;
    }

    [[nodiscard]] std::optional<Side> collidedScreenEdgeHorizontal() const {
        if (x + RADIUS >= screen->x)
            return Side::RIGHT;
        else if (x - RADIUS <= 0)
            return Side::LEFT;
        return std::nullopt;
    }

    [[nodiscard]] std::optional<Side> collidedPaddle() const {
        // Right paddle
        if (x + RADIUS >= rightPaddle->x && y >= rightPaddle->y && y <= rightPaddle->y + rightPaddle->h) {
            return Side::RIGHT;
        }
        // Left paddle
        if (x - RADIUS <= leftPaddle->x + leftPaddle->w && y >= leftPaddle->y && y <= leftPaddle->y + leftPaddle->h) {
            return Side::LEFT;
        }
        return std::nullopt;
    }

};


