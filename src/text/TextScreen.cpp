#include <string>
#include <sstream>
#include <utility>
#include "TextScreen.h"
#include "optional"

std::vector<std::string> splitString(const std::string &string, const char &delim) {
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

TextScreen::TextScreen(const std::string &text, SDL_Point *const screenSize, std::optional<SDL_Point> basePosition)
        : hasUpdated(false), screenSize(screenSize), basePosition(basePosition) {
    const char *defaultFontPath = getDefaultFontPath();
    if (defaultFontPath == nullptr) {
        std::cerr << "Font path is not set for this platform (null)" << std::endl;
        exit(-1);
    }
    font = TTF_OpenFont(defaultFontPath, 42);
    if (font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        exit(-1);
    }

    initPositions(text);
}

void TextScreen::initPositions(const std::string &text) {
    lines = splitString(text, '\n');
    surfaces.clear();
    shadowSurfaces.clear();
    positions.clear();

    surfaces.reserve(lines.size());
    shadowSurfaces.reserve(lines.size());
    positions.reserve(lines.size());

    for (int i = 0; i < lines.size(); ++i) {
        auto pair = calculatePositionOf(i);
        positions.push_back(pair);
    }
}

Position TextScreen::calculatePositionOf(const int i) const {
    int textWidth, textHeight;
    TTF_SizeText(font, lines[i].c_str(), &textWidth, &textHeight);

    SDL_Point base = basePosition.has_value() ? basePosition.value() : SDL_Point{(screenSize->x - textWidth) / 2,
                                                                                 static_cast<int>((screenSize->y -
                                                                                                   textHeight *
                                                                                                   (lines.size())) /
                                                                                                  2)};

    SDL_Rect regularPosition = {base.x, base.y + textHeight * i, textWidth, textHeight};
    SDL_Rect shadowPosition = {base.x + shadowOffset, base.y + textHeight * i + shadowOffset, textWidth,
                               textHeight};
    return Position{.shadowPosition=shadowPosition, .regularPosition=regularPosition};

}

TextScreen::~TextScreen() {
    // TTF_CLoseFont & SDL_FreeSurface are null-safe
    TTF_CloseFont(font);
    for (auto *surface: surfaces)
        SDL_FreeSurface(surface);
    for (auto *surface: shadowSurfaces)
        SDL_FreeSurface(surface);
}

void TextScreen::draw(SDL_Renderer *const renderer) {
    for (int i = 0; i < surfaces.size(); ++i) {
        // Draw shadow
        SDL_Texture *shadowTexture = SDL_CreateTextureFromSurface(renderer, shadowSurfaces[i]);
        if (shadowTexture != nullptr) {
            SDL_RenderCopy(renderer, shadowTexture, nullptr, &positions[i].shadowPosition);
            SDL_DestroyTexture(shadowTexture);
        }

        // Draw text
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaces[i]);
        if (texture != nullptr) {
            SDL_RenderCopy(renderer, texture, nullptr, &positions[i].regularPosition);
            SDL_DestroyTexture(texture);
        }
    }
}

void TextScreen::setText(const std::string &replaceText) {
    lines = splitString(replaceText, '\n');
    initPositions(replaceText);
}


void TextScreen::update() {
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

/**
 *
 * @param c to replace with
 * @param line Should be zero indexed. That means -1 from getAmountOfLines
 * @param column Should be zero indexed. That means -1 from getAmountOfColumns
 */
void TextScreen::replaceLine(const size_t index, std::string line) {
    lines[index] = std::move(line);
    hasUpdated = false;
    positions[index] = calculatePositionOf(index);
}

std::string &TextScreen::getLine(const size_t index) {
    return lines[index];
}

size_t TextScreen::getAmountOfLines() {
    return lines.size();
}

