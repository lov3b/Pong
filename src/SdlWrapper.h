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
    explicit SdlWrapper(const char *title, const SDL_Point screenSize, const uint8_t fps) : fps(fps) {
        this->screenSize = screenSize;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            exit(-1);
        }
        if (TTF_Init() < 0) {
            std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
            exit(-1);
        }
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.x, screenSize.y,
                                  0);
        if (window == nullptr) {
            std::cerr << "Failed to create SDL_Window with error: " << SDL_GetError() << std::endl;
            exit(-1);
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr) {
            std::cerr << "Failed to create SDL_Renderer with error: " << SDL_GetError() << std::endl;
            exit(-1);
        }

        SDL_RWops *ops = SDL_RWFromConstMem(icon, iconLength);
        if (ops == nullptr) {
            std::cerr << "Failed to load from constant memory with error: " << SDL_GetError() << std::endl;
            exit(-1);
        }
        iconSurface = SDL_LoadBMP_RW(ops, 1);
        if (iconSurface == nullptr) {
            std::cerr << "Failed to load BMP from SDL_RWops with error: " << SDL_GetError() << std::endl;
            exit(-1);
        }
        SDL_SetWindowIcon(window, iconSurface);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    virtual ~SdlWrapper() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_FreeSurface(iconSurface);
        SDL_Quit();
    }

    virtual void draw(SDL_Renderer *renderer) = 0;

    virtual void update() = 0;

    virtual bool handleEvents() = 0;

    int loop() {
        while (running) {
            if (!handleEvents())
                break;
            update();
            draw(renderer);
            SDL_Delay(1000 / fps);
        }

        return 1;
    }
};
