// SPDX-License-Identifier: BSD-2-Clause

#include "Ball.h"


Ball::Ball(const SDL_Point *screen, const PlayerPaddle *leftPaddle, const PlayerPaddle *rightPaddle, Score *score)
        : score(score), screen(screen), leftPaddle(leftPaddle), rightPaddle(rightPaddle),
          x(screen->x / 2), y(screen->y / 2), vec2d(new Vec2d(6)) {
}

void Ball::resetPosition() {
    this->x = screen->x / 2;
    this->y = screen->y / 2;

    delete vec2d;
    vec2d = new Vec2d(6);
}

void Ball::draw(SDL_Renderer *const renderer) const {
    filledCircleColor(renderer, x, y, RADIUS, color);
}

void Ball::update() {
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

bool Ball::collidedScreenEdgeVertical() const {
    return y - RADIUS <= 0 || y + RADIUS >= screen->y;
}

std::optional<Side> Ball::collidedScreenEdgeHorizontal() const {
    if (x + RADIUS >= screen->x)
        return Side::RIGHT;
    else if (x - RADIUS <= 0)
        return Side::LEFT;
    return std::nullopt;
}

std::optional<Side> Ball::collidedPaddle() const {
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
