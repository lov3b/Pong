//
// Created by love on 2024-01-19.
//

#pragma once

#include <cstdint>
#include <SDL_render.h>
#include <functional>
#include "Side.h"
#include "SDL_ttf.h"


#include <SDL_ttf.h>
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
// Windows
    const char* defaultFontPath = "C:\\Windows\\Fonts\\Arial.ttf";
#elif defined(__linux__)
const char *defaultFontPath = "/usr/share/fonts/truetype/DejaVuSans-Bold.ttf";
#else
// Other platforms
    const char* defaultFontPath = "path/to/a/default/font.ttf";
#endif


class Score {
private:
    uint8_t leftScore, rightScore;
    const uint8_t MAX_SCORE;
    std::function<void(Side)> &whenWon;
    TTF_Font *font;
    bool hasIncremented = false;

    // Regular
    SDL_Rect position;
    SDL_Color color = {243, 156, 18, 255};
    SDL_Surface *surface = nullptr;

    // Shadow
    const SDL_Color shadowColor = {243, 156, 18, 100}; // Black color for the shadow
    SDL_Surface *shadowSurface = nullptr;
    const int shadowOffset = 3;

public:
    explicit Score(uint8_t max_score, SDL_Point *screenSize, std::function<void(Side)> whenWon) : MAX_SCORE(max_score),
                                                                                                  whenWon(whenWon) {
        resetScore();
        this->font = TTF_OpenFont(defaultFontPath, 42);
        if (font == nullptr) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            exit(-1);
        }

        this->position = SDL_Rect{screenSize->x / 2 - 50, 10, 100, 40};
    }

    ~Score() {
        if (font)
            TTF_CloseFont(font);
        if (surface)
            SDL_FreeSurface(surface);
    }

    void update() {
        if (!hasIncremented && surface != nullptr && shadowSurface != nullptr) return;

        if (surface != nullptr)
            SDL_FreeSurface(surface);
        if (shadowSurface != nullptr)
            SDL_FreeSurface(shadowSurface);

        hasIncremented = false;
        char score_text[8];
        sprintf(score_text, "%d - %d", leftScore, rightScore);

        // Create shadow surface
        shadowSurface = TTF_RenderText_Solid(font, score_text, shadowColor);
        if (shadowSurface == nullptr) {
            std::cerr << "Failed to create shadow text surface: " << TTF_GetError() << std::endl;
        }

        // Create main text surface
        surface = TTF_RenderText_Solid(font, score_text, color);
        if (surface == nullptr) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        }
    }

    void draw(SDL_Renderer *renderer) {
        // Draw shadow
        if (shadowSurface != nullptr) {
            SDL_Texture *shadowTexture = SDL_CreateTextureFromSurface(renderer, shadowSurface);
            if (shadowTexture != nullptr) {
                SDL_Rect shadowPosition = {position.x + shadowOffset, position.y + shadowOffset, position.w,
                                           position.h};
                SDL_RenderCopy(renderer, shadowTexture, NULL, &shadowPosition);
                SDL_DestroyTexture(shadowTexture);
            }
        }

        // Draw text
        if (surface != nullptr) {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture != nullptr) {
                SDL_RenderCopy(renderer, texture, NULL, &position);
                SDL_DestroyTexture(texture);
            }
        }
    }

    void resetScore() {
        leftScore = rightScore = 0;
    }

    void incrementScore(Side side) {
        hasIncremented = true;
        uint8_t temp;
        switch (side) {
            case Side::LEFT:
                temp = ++leftScore;
                break;
            case Side::RIGHT:
                temp = ++rightScore;
                break;
        }

        if (temp > MAX_SCORE)
            whenWon(side);
    }

};
