// SPDX-License-Identifier: BSD-2-Clause

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
                  Score *score);

    void resetPosition();

    void draw(SDL_Renderer *renderer) const;

    void update();

private:
    [[nodiscard]] bool collidedScreenEdgeVertical() const;

    [[nodiscard]] std::optional<Side> collidedScreenEdgeHorizontal() const;

    [[nodiscard]] std::optional<Side> collidedPaddle() const;

};


