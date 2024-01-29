// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include "Side.h"
#include "../Vec2d/Bump.h"

class PlayerPaddle : public SDL_Rect {
private:
    static const int MOVE_PER_TICK = 5;
    const SDL_Point *screen;
    bool movingUp = false, movingDown = false;
    uint8_t color[4]{};

public:
    PlayerPaddle(const SDL_Point *screen, Side side);

    [[nodiscard]] PaddleDirection getPaddleDirection() const;

    void draw(SDL_Renderer *renderer) const;

    void startMoving(bool up);

    void stopMoving(bool up);

    void update();

private:
    [[nodiscard]] bool canMoveDown() const;

    [[nodiscard]] bool canMoveUp() const;
};
