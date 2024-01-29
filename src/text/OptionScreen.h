// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "TextScreen.h"
#include "../VisibleObjects/Side.h"


class OptionScreen : public TextScreen {
private:
    bool hasStartedCounting_ = false;
    int_least64_t nextMsEpoch = 0;
    int stepsToDo, stepsDone = 0;
    bool isDone_ = false;

    TTF_Font *secondaryFont;
public:
    [[nodiscard]] const bool &isDone() const {
        return isDone_;
    }

    [[nodiscard]] const bool &hasStartedCounting() const {
        return hasStartedCounting_;
    }

    OptionScreen(const std::string &text, SDL_Point *screenSize, int seconds);

    void update() override;

    void startCountDown();
};

