//
// Created by love on 2024-01-18.
//

#pragma

#include <string>
#include <SDL_ttf.h>
#include <iostream>
#include "SDL.h"
#include "icon.h"

class SdlWrapper {
private:
    const uint8_t fps = 60;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *iconSurface;
protected:
    SDL_Point screenSize;
    bool running = true;

public:
    explicit SdlWrapper(const char *title, SDL_Point screenSize, uint8_t fps);

    virtual ~SdlWrapper();

    int loop();

protected:
    virtual void draw(SDL_Renderer *renderer) = 0;

    virtual void update() = 0;

    virtual bool handleEvents() = 0;
};
