//
// Created by love on 2024-01-24.
//

#pragma once
#include <filesystem>


const char* getLinuxFilePath() {
    const char *fonts[] = {
            "/usr/share/fonts/truetype/DejaVuSans-Bold.ttf", // openSUSE
            "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", // Debian
            "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", // Arch
            "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans-Bold.ttf", // Fedora
    };
    for (const char *font : fonts)
        if (std::filesystem::exists(font))
            return font;

    return nullptr;
}

#if defined(_WIN32) || defined(_WIN64)
const char* defaultFontPath = "C:\\Windows\\Fonts\\Arial.ttf";
#elif defined(__linux__)
const char *defaultFontPath = getLinuxFilePath();
#elif defined(__APPLE__) || defined(__MACH__)
const char *defaultFontPath = "/System/Library/Fonts/Supplemental/Arial.ttf";
#else
const char* defaultFontPath = nullptr;
#endif

