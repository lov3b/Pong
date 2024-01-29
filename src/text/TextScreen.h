//
// Created by love on 2024-01-24.
//

#pragma once

#include <SDL_render.h>
#include <SDL_ttf.h>
#include <utility>
#include <vector>
#include <optional>
#include "../defaultfont.h"
#include "iostream"


class TextScreen {
private:
    std::vector<SDL_Surface *> surfaces;
    std::vector<SDL_Surface *> shadowSurfaces;
    TTF_Font *font;
    SDL_Point *screenSize;
    std::optional<SDL_Point> basePosition;

    // Regular
    std::vector<SDL_Rect> positions;
    SDL_Color color = {243, 156, 18, 255};

    // Shadow
    std::vector<SDL_Rect> shadowPositions;
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

    virtual void update();

private:
    void initPositions(const std::string &text);
};
