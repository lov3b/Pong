//
// Created by love on 2024-01-19.
//

#pragma once

#include "random"
#include "ctime"
#include "cmath"

#include "SDL_rect.h"
#include "Bump.h"

inline double_t toRadians(double_t degrees) {
    return degrees * M_PI / 100;
}

class Vec2d {
private:
    std::mt19937 mtRand;
    std::uniform_real_distribution<double_t> smallAngleGen;
    float_t hypotenuse;
    double_t x, y;
    const float_t bumpSpeedIncrease = 1.05;

public:
    explicit Vec2d(float_t hypotenuse);

    void applyVector(Sint16 *ox, Sint16 *oy) const;

    void bump(BumpType bumpType, PaddleDirection paddleDirection);
};