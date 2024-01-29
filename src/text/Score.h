//
// Created by Love on 2024-01-29.
//
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

class Score : public TextScreen {
private:
    const uint8_t MAX_SCORE;
    std::optional<Side> sideWon_;

public:
    uint8_t leftScore, rightScore;

    [[nodiscard]] const std::optional<Side> &sideWon() const {
        return sideWon_;
    }

public:
    explicit Score(SDL_Point *screenSize, uint8_t max_score) : MAX_SCORE(max_score), leftScore(0), rightScore(0),
                                                               TextScreen("", screenSize, std::make_optional(
                                                                       SDL_Point{screenSize->x / 2 - 50, 10})) {
    }

    void update() override {
        if (hasUpdated) return;

        std::stringstream ss;
        ss << std::to_string(leftScore) << " - " << std::to_string(rightScore);
        setText(ss.str());
        TextScreen::update();

        hasUpdated = true;
    }

    void resetScore() {
        leftScore = rightScore = 0;
        sideWon_ = std::nullopt;
        hasUpdated = false;
    }

    void incrementScore(const Side side) {
        hasUpdated = false;
        uint8_t incrementedScore;
        switch (side) {
            case Side::LEFT:
                incrementedScore = ++leftScore;
                break;
            case Side::RIGHT:
                incrementedScore = ++rightScore;
                break;
        }

        if (incrementedScore > MAX_SCORE)
            sideWon_ = side;
    }

};

