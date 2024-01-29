//
// Created by love on 2024-01-19.
//

#pragma once


#include "Side.h"
#include "../Vec2d/Bump.h"

class PlayerPaddle : public SDL_Rect {
private:
    static const int MOVE_PER_TICK = 5;
    const SDL_Point *screen;
    bool movingUp, movingDown;
    uint8_t color[4]{};

public:
    PlayerPaddle(const SDL_Point *screen, const Side side) : SDL_Rect() {
        w = 20;
        h = 80;
        x = side == Side::LEFT ? 0 : screen->x - w;
        y = (screen->y - h) / 2;
        movingUp = false;
        movingDown = false;
        this->screen = screen;

        color[0] = 255;
        color[1] = 234;
        color[2] = 0;
        color[3] = 255;
    }

    [[nodiscard]] PaddleDirection getPaddleDirection() const {
        if (movingUp != movingDown)
            return PaddleDirection::NOT_MOVING;
        else if (movingUp)
            return PaddleDirection::MOVING_UP;
        return PaddleDirection::MOVING_DOWN;
    }

    void draw(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderFillRect(renderer, this);
    }

    void startMoving(const bool up) {
        if (up)
            movingUp = true;
        else movingDown = true;
    }

    void stopMoving(const bool up) {
        if (up)
            movingUp = false;
        else movingDown = false;
    }

    void update() {
        // We cannot move up and down
        if (movingUp == movingDown)
            return;

        if (movingUp && canMoveUp())
            y -= MOVE_PER_TICK;
        else if (movingDown && canMoveDown())
            y += MOVE_PER_TICK;
    }

private:
    [[nodiscard]] bool canMoveDown() const {
        return y + h < screen->y;
    }

    [[nodiscard]] bool canMoveUp() const {
        return y > 0;
    }

};
