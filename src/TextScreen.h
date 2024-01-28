//
// Created by love on 2024-01-24.
//

#pragma once

#include <SDL_render.h>
#include <SDL_ttf.h>
#include "defaultfont.h"
#include "iostream"

class TextScreen {
private:
    std::string *text;
    TTF_Font *font;
    bool hasUpdated;

    // Regular
    SDL_Rect position;
    SDL_Color color = {243, 156, 18, 255};
    SDL_Surface *surface = nullptr;

    // Shadow
    const SDL_Color shadowColor = {243, 156, 18, 100};
    SDL_Surface *shadowSurface = nullptr;
    const int shadowOffset = 3;

public:
    /**
     *
     * @param text This class takes care of freeing text
     * @param screenSize This won't be freed by this class
     */
    TextScreen(std::string *text, SDL_Point *screenSize) : text(text), hasUpdated(false) {
        if (defaultFontPath == nullptr) {
            std::cerr << "Font path is not set for this platform (null)" << std::endl;
            exit(-1);
        }
        this->font = TTF_OpenFont(defaultFontPath, 42);
        if (font == nullptr) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            exit(-1);
        }
        this->position = SDL_Rect{50, 50, screenSize->x - 50, screenSize->y - 50};
    }

    ~TextScreen() {
        if (font)
            TTF_CloseFont(font);
        SDL_FreeSurface(surface);
        delete text;
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

    void setText(std::string *replaceText) {
        delete this->text;
        this->text = replaceText;
    }

    void replaceCharAtIndex(char c, int index) {
        if (text->length() <= index) {
            std::cerr << "text string is at length " << text->length() << ", but index is " << index << std::endl;
            return;
        }
        hasUpdated = false;
        text[index] = c;
    }

    void update() {
        if (!hasUpdated)
            return;

        SDL_FreeSurface(surface);
        SDL_FreeSurface(shadowSurface);

        shadowSurface = TTF_RenderText_Solid(font, text->c_str(), shadowColor);
        if (shadowSurface == nullptr)
            std::cerr << "Failed to create shadow text surface (TextScreen): " << TTF_GetError() << std::endl;

        surface = TTF_RenderText_Solid(font, text->c_str(), color);
        if (surface == nullptr)
            std::cerr << "Failed to create text surface (TextScreen): " << TTF_GetError() << std::endl;

        hasUpdated = true;

    }

    bool gameContextFinished() {

    }

};
