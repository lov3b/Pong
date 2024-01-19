//
// Created by love on 2024-01-18.
//

#pragma

#include <string>
#include "SDL.h"

class SdlWrapper {
private:
    const uint8_t fps = 60;
    SDL_Window *window;
    SDL_Renderer *renderer;
protected:
    SDL_Point screenSize;

public:
    explicit SdlWrapper(const char *title, const SDL_Point screenSize, const uint8_t fps) : fps(fps) {
        this->screenSize = screenSize;

        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(
                title,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                screenSize.x,
                screenSize.y,
                0
        );
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    virtual ~SdlWrapper() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    virtual void draw(SDL_Renderer *renderer) = 0;

    virtual bool update() = 0;

    virtual bool handleEvents() = 0;

    int loop() {
        while (true) {
            if (!handleEvents() || !update())
                break;
            draw(renderer);
            SDL_Delay(1000 / fps);
        }

        return 1;
    }
};
