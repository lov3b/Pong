// SPDX-License-Identifier: BSD-2-Clause

#include "OptionScreen.h"
#include <chrono>


int_least64_t getCurrentEpochTimeMillis() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto now_ms = time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = duration_cast<std::chrono::milliseconds>(epoch);
    return value.count();
}

OptionScreen::OptionScreen(const std::string &text, SDL_Point *screenSize, int seconds) : TextScreen(text, screenSize,
                                                                                                     std::nullopt),
                                                                                          stepsToDo(seconds) {
}

void OptionScreen::update() {
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

void OptionScreen::startCountDown() {
    auto epochMs = getCurrentEpochTimeMillis();
    nextMsEpoch = epochMs;
    hasStartedCounting_ = true;
    stepsDone = 0;
}