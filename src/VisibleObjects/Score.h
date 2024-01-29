//
// Created by love on 2024-01-19.
//

#pragma once

#include <cstdint>
#include <SDL_render.h>
#include <functional>
#include "Side.h"
#include "SDL_ttf.h"
#include "../defaultfont.h"


#include <SDL_ttf.h>
#include <string>
#include <iostream>


class Score {
private:
    uint8_t leftScore, rightScore;
    const uint8_t MAX_SCORE;
    const std::function<void(Side)> whenWon;
    TTF_Font *font;
    bool hasIncremented = false;

    // Regular
    SDL_Rect position;
    SDL_Color color = {243, 156, 18, 255};
    SDL_Surface *surface = nullptr;

    // Shadow
    const SDL_Color shadowColor = {243, 156, 18, 100};
    SDL_Surface *shadowSurface = nullptr;
    const int shadowOffset = 3;

public:
    explicit Score(uint8_t max_score, SDL_Point *screenSize, const std::function<void(Side)> &whenWon) : MAX_SCORE(
            max_score), whenWon(whenWon) {
        resetScore();
        if (defaultFontPath == nullptr) {
            std::cerr << "Font path is not set for this platform (null)" << std::endl;
            exit(-1);
        }

        this->font = TTF_OpenFont(defaultFontPath, 42);
        if (font == nullptr) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            exit(-1);
        }

        this->position = SDL_Rect{screenSize->x / 2 - 50, 10, 100, 50};
        this->rightScore = this->leftScore = 0;
    }

    ~Score() {
        if (font)
            TTF_CloseFont(font);
        SDL_FreeSurface(surface);
    }

    void update() {
        if (!hasIncremented && surface != nullptr && shadowSurface != nullptr) return;

        SDL_FreeSurface(surface);
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
                SDL_RenderCopy(renderer, shadowTexture, nullptr, &shadowPosition);
                SDL_DestroyTexture(shadowTexture);
            }
        }

        // Draw text
        if (surface != nullptr) {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture != nullptr) {
                SDL_RenderCopy(renderer, texture, nullptr, &position);
                SDL_DestroyTexture(texture);
            }
        }
    }

    void resetScore() {
        leftScore = rightScore = 0;
    }

    void incrementScore(const Side side) {
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
