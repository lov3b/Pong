// SPDX-License-Identifier: BSD-2-Clause

#include "SdlWrapper.h"

SdlWrapper::SdlWrapper(const char *const title, const SDL_Point screenSize, const uint8_t fps) : fps(fps), screenSize(
        screenSize) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(-1);
    }
    if (TTF_Init() < 0) {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        exit(-1);
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.x, screenSize.y, 0);
    if (window == nullptr) {
        std::cerr << "Failed to create SDL_Window with error: " << SDL_GetError() << std::endl;
        exit(-1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Failed to create SDL_Renderer with error: " << SDL_GetError() << std::endl;
        exit(-1);
    }
    SDL_RendererInfo rendererInfo;
    if (SDL_GetRendererInfo(renderer, &rendererInfo) == 0) {
        std::cout << "Using renderer: " << rendererInfo.name << std::endl;
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

SdlWrapper::~SdlWrapper() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(iconSurface);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

bool SdlWrapper::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
                handleKeyDown(event);
                break;
            case SDL_KEYUP:
                handleKeyUp(event);
                break;
        }
    }

    return true;
}

int SdlWrapper::loop() {
    while (running) {
        if (!handleEvents())
            break;
        update();
        draw(renderer);
        SDL_Delay(1000 / fps);
    }

    return 1;
}
