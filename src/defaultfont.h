//
// Created by love on 2024-01-24.
//

#pragma once

#if defined(_WIN32) || defined(_WIN64)
const char* defaultFontPath = "C:\\Windows\\Fonts\\Arial.ttf";
#elif defined(__linux__)
const char *defaultFontPath = "/usr/share/fonts/truetype/DejaVuSans-Bold.ttf";
#elif defined(__APPLE__) || defined(__MACH__)
const char *defaultFontPath = "/System/Library/Fonts/Supplemental/Arial.ttf";
#else
const char* defaultFontPath = nullptr;
#endif

