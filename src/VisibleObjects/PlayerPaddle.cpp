// SPDX-License-Identifier: BSD-2-Clause

#include "PlayerPaddle.h"


PlayerPaddle::PlayerPaddle(const SDL_Point *screen, const Side side) : SDL_Rect(), screen(screen) {
    w = 20;
    h = 80;
    x = side == Side::LEFT ? 20 : screen->x - w-20;
    y = (screen->y - h) / 2;

    color[0] = 255;
    color[1] = 234;
    color[2] = 0;
    color[3] = 255;
}

[[nodiscard]] PaddleDirection PlayerPaddle::getPaddleDirection() const {
    if (movingUp != movingDown)
        return PaddleDirection::NOT_MOVING;
    else if (movingUp)
        return PaddleDirection::MOVING_UP;
    return PaddleDirection::MOVING_DOWN;
}

void PlayerPaddle::draw(SDL_Renderer *renderer) const {
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
    SDL_RenderFillRect(renderer, this);
}

void PlayerPaddle::update() {
    // We cannot move up and down
    if (movingUp == movingDown)
        return;

    if (movingUp && canMoveUp())
        y -= MOVE_PER_TICK;
    else if (movingDown && canMoveDown())
        y += MOVE_PER_TICK;
}

void PlayerPaddle::startMoving(const bool up) {
    if (up)
        movingUp = true;
    else movingDown = true;
}

void PlayerPaddle::stopMoving(const bool up) {
    if (up)
        movingUp = false;
    else movingDown = false;
}

[[nodiscard]] bool PlayerPaddle::canMoveDown() const {
    return y + h < screen->y;
}

[[nodiscard]] bool PlayerPaddle::canMoveUp() const {
    return y > 0;
}