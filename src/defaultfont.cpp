// SPDX-License-Identifier: BSD-2-Clause

#if defined(__LINUX__)
#include <filesystem>

const char *getLinuxFilePath() {
    const char *fonts[] = {"/usr/share/fonts/truetype/DejaVuSans-Bold.ttf", // openSUSE
                           "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", // Debian
                           "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", // Arch
                           "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans-Bold.ttf", // Fedora
    };
    for (const char *font: fonts)
        if (std::filesystem::exists(font))
            return font;

    return nullptr;
}
#endif


const char *getDefaultFontPath() {
#if defined(_WIN32) || defined(_WIN64)
    return R"(C:\Windows\Fonts\Arial.ttf)";
#elif defined(__linux__)
    return getLinuxFilePath();
#elif defined(__APPLE__) || defined(__MACH__)
    return "/System/Library/Fonts/Supplemental/Arial.ttf";
#else
    return nullptr;
#endif
}

