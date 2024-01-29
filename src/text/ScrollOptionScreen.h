// SPDX-License-Identifier: BSD-2-Clause

#pragma once


#include "OptionScreen.h"

class ScrollOptionScreen : public OptionScreen {

private:
    uint8_t &counter;
    bool shouldUpdate = true;

public:
    ScrollOptionScreen(const std::string &text, SDL_Point *screenSize, int seconds, uint8_t &counterRef);

    void handleEvent(SDL_Event &event);

    void update() override;
};


