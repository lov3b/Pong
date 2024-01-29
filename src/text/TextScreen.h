// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <SDL_render.h>
#include <SDL_ttf.h>
#include <utility>
#include <vector>
#include <optional>
#include "../defaultfont.h"
#include "iostream"

struct Position {
    SDL_Rect shadowPosition;
    SDL_Rect regularPosition;
};

class TextScreen {
private:
    std::vector<SDL_Surface *> surfaces;
    std::vector<SDL_Surface *> shadowSurfaces;
    TTF_Font *font;
    SDL_Point *screenSize;
    std::optional<SDL_Point> basePosition;

    // Regular
    std::vector<Position> positions;
    SDL_Color color = {243, 156, 18, 255};

    // Shadow
    const SDL_Color shadowColor = {243, 156, 18, 100};
    const int shadowOffset = 3;

protected:
    std::vector<std::string> lines;
    bool hasUpdated;

public:
    /**
     *
     * @param text This class takes care of freeing text
     * @param screenSize This won't be freed by this class
     */
    TextScreen(const std::string &text, SDL_Point *screenSize, std::optional<SDL_Point> basePosition);

    ~TextScreen();

    virtual void draw(SDL_Renderer *renderer);

    void setText(const std::string &replaceText);

    void replaceLine(size_t index, std::string line);

    std::string &getLine(size_t index);

    size_t getAmountOfLines();

    virtual void update();

private:
    void initPositions(const std::string &text);

    Position calculatePositionOf(int i) const;

};
