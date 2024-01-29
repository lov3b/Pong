//
// Created by love on 2024-01-24.
//

#pragma once

#include <SDL_render.h>
#include <SDL_ttf.h>
#include <utility>
#include <vector>
#include "defaultfont.h"
#include "iostream"


class TextScreen {
private:
    std::vector<std::string> lines;
    TTF_Font *font;
    SDL_Point *screenSize;

    // Regular
    std::vector<SDL_Rect> positions;
    SDL_Color color = {243, 156, 18, 255};
    std::vector<SDL_Surface *> surfaces;

    // Shadow
    std::vector<SDL_Rect> shadowPositions;
    const SDL_Color shadowColor = {243, 156, 18, 100};
    std::vector<SDL_Surface *> shadowSurfaces;
    const int shadowOffset = 3;

protected:
    bool hasUpdated;

public:
    /**
     *
     * @param text This class takes care of freeing text
     * @param screenSize This won't be freed by this class
     */
    TextScreen(const std::string &text, SDL_Point *screenSize) : hasUpdated(false), screenSize(screenSize) {
        if (defaultFontPath == nullptr) {
            std::cerr << "Font path is not set for this platform (null)" << std::endl;
            exit(-1);
        }
        this->font = TTF_OpenFont(defaultFontPath, 42);
        if (font == nullptr) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            exit(-1);
        }

        initPositions(text);
    }

private:
    void initPositions(const std::string &text) {
        lines = splitString(text, '\n');
        surfaces.clear();
        shadowSurfaces.clear();
        positions.clear();
        shadowPositions.clear();

        surfaces.reserve(lines.size());
        shadowSurfaces.reserve(lines.size());
        positions.reserve(lines.size());
        shadowPositions.reserve(lines.size());

        for (int i = 0; i < lines.size(); ++i) {
            int textWidth, textHeight;
            TTF_SizeText(font, lines[i].c_str(), &textWidth, &textHeight);

            int baseX = (screenSize->x - textWidth) / 2, baseY = (screenSize->y - textHeight * (lines.size())) / 2;
            SDL_Rect regularPosition = {baseX,
                                        baseY + textHeight * i,
                                        textWidth, textHeight};
            SDL_Rect shadowPosition = {baseX + shadowOffset,
                                       baseY + textHeight * i + shadowOffset,
                                       textWidth,
                                       textHeight};
            positions.push_back(regularPosition);
            shadowPositions.push_back(shadowPosition);
        }
    }

public:
    ~TextScreen() {
        if (font)
            TTF_CloseFont(font);
        for (auto *surface: surfaces)
            SDL_FreeSurface(surface);
        for (auto *surface: shadowSurfaces)
            SDL_FreeSurface(surface);
    }


    virtual void draw(SDL_Renderer *renderer) {
        for (int i = 0; i < surfaces.size(); ++i) {
            // Draw shadow
            SDL_Texture *shadowTexture = SDL_CreateTextureFromSurface(renderer, shadowSurfaces[i]);
            if (shadowTexture != nullptr) {
                SDL_RenderCopy(renderer, shadowTexture, nullptr, &shadowPositions[i]);
                SDL_DestroyTexture(shadowTexture);
            }

            // Draw text
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaces[i]);
            if (texture != nullptr) {
                SDL_RenderCopy(renderer, texture, nullptr, &positions[i]);
                SDL_DestroyTexture(texture);
            }
        }
    }

    void setText(const std::string &replaceText) {
        lines = splitString(replaceText, '\n');
        initPositions(replaceText);
    }

    void replaceCharAtIndex(char c, int line, int index) {
        if (lines.size() <= line) {
            if (lines[line].length() <= index) {
                std::cerr << "string lines is of length " << lines.size() << ", but line index is " << index
                          << std::endl;
                return;
            }
        }
        if (lines[line].length() <= index) {
            std::cerr << "text string is of length " << lines[line].length() << ", but index is " << index << std::endl;
            return;
        }
        hasUpdated = false;
        lines[line][index] = c;
    }

    virtual void update() {
        if (hasUpdated)
            return;

        for (auto &surface: surfaces)
            SDL_FreeSurface(surface);
        for (auto &shadowSurface: shadowSurfaces)
            SDL_FreeSurface(shadowSurface);
        surfaces.clear();
        shadowSurfaces.clear();

        for (const auto &line: lines) {
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, line.c_str(), color);
            SDL_Surface *shadowSurface = TTF_RenderText_Solid(font, line.c_str(), shadowColor);

            if (textSurface == nullptr || shadowSurface == nullptr) {
                std::cerr << "Failed to create text surface (TextScreen): " << TTF_GetError() << std::endl;
                continue;
            }

            surfaces.push_back(textSurface);
            shadowSurfaces.push_back(shadowSurface);

        }

        hasUpdated = true;
    }

private:
    static std::vector<std::string> splitString(const std::string &string, const char delim) {
        int size = 0;
        for (char c: string)
            if (c == delim) size++;

        std::vector<std::string> lines;
        lines.reserve(size);

        std::stringstream ss(string);
        std::string line;
        while (std::getline(ss, line, delim))
            lines.push_back(line);


        return lines;
    }
};
