//
// Created by Love on 2024-01-29.
//

#pragma once

#include "TextScreen.h"
#include "chrono"

int_least64_t getCurrentEpochTimeMillis() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    return value.count();
}


class OptionScreen : public TextScreen {
private:
    bool hasStartedCounting_ = false;
    int_least64_t nextMsEpoch = 0;
    int stepsToDo, stepsDone = 0;
    bool isDone_ = false;

public:
    [[nodiscard]] const bool &isDone() const {
        return isDone_;
    }

    [[nodiscard]] const bool &hasStartedCounting() const {
        return hasStartedCounting_;
    }

public:
    OptionScreen(const std::string &text, SDL_Point *screenSize, int seconds) : TextScreen(text, screenSize,
                                                                                           std::nullopt),
                                                                                stepsToDo(seconds) {
    }

    void update() override {
        auto now = getCurrentEpochTimeMillis();
        if (hasStartedCounting_ && nextMsEpoch <= now) {
            if (stepsDone < stepsToDo) {
                std::string s = std::to_string(stepsToDo - stepsDone);
                setText(s);
                nextMsEpoch = now + 1000;
                hasUpdated = false;
                stepsDone++;
            } else {
                isDone_ = true;
            }
        }

        TextScreen::update();
    }

    void startCountDown() {
        auto epochMs = getCurrentEpochTimeMillis();
        nextMsEpoch = epochMs + 1000;
        hasStartedCounting_ = true;
        stepsDone = 0;
    }

};

