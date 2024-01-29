//
// Created by Love on 2024-01-29.
//

#include "ScrollOptionScreen.h"

std::string textAppendHelper(std::string string, const std::string &toAppend) {
    string.append("\nMax Score: ");
    string.append(toAppend);
    return std::move(string);
}

ScrollOptionScreen::ScrollOptionScreen(const std::string &text, SDL_Point *screenSize, int seconds, uint8_t &counterRef)
        : OptionScreen(textAppendHelper(text, std::to_string(counterRef)), screenSize, seconds),
          counter(counterRef) {
}


void ScrollOptionScreen::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                [[fallthrough]];
            case SDLK_UP:
                counter++;
                shouldUpdate = true;
                break;
            case SDLK_s:
                [[fallthrough]];
            case SDLK_DOWN:
                if (counter > 0) {
                    counter--;
                    shouldUpdate = true;
                }
                break;
            case SDLK_RETURN:
                shouldUpdate = true;
                startCountDown();
                break;
        }
    }
}

void ScrollOptionScreen::update() {
    if (shouldUpdate) {
        size_t lineIndex = getAmountOfLines() - 1;
        std::string line = getLine(lineIndex);
        size_t lastSpace = line.rfind(' ');
        if (lastSpace == std::string::npos) {
            std::cerr << "Could not find the last space..." << std::endl;
            return;
        }
        line.erase(lastSpace + 1);
        line.append(std::to_string(counter));

        replaceLine(lineIndex, std::move(line));
        shouldUpdate = false;
    }

    OptionScreen::update();
}
