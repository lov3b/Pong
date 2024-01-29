// SPDX-License-Identifier: BSD-2-Clause

#pragma once


#include "TextScreen.h"
#include <cstdint>
#include <SDL_render.h>
#include <functional>
#include "../VisibleObjects/Side.h"
#include "SDL_ttf.h"
#include "../defaultfont.h"
#include <string>
#include <iostream>
#include <optional>
#include <sstream>

class Score : public TextScreen {
private:
    uint8_t maxScore_;
    std::optional<Side> sideWon_;

public:
    uint8_t leftScore, rightScore;

    [[nodiscard]] const std::optional<Side> &sideWon() const {
        return sideWon_;
    }

public:
    explicit Score(SDL_Point *screenSize, uint8_t max_score);

    void update() override;

    void resetScore();

    void incrementScore(const Side side);

    uint8_t &maxScore() {
        return maxScore_;
    }
};

