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
    std::default_random_engine random;
    const float_t hypotenuse;
    double_t x, y;

public:
    Vec2d(float_t hypotenuse) : hypotenuse(hypotenuse) {
        std::random_device rd;
        random = std::default_random_engine(rd());
        int sign = random() % 2 == 0 ? -1 : 1;
        //double_t angle = toRadians(random() % 6000 / 100 - 30);
        double_t angle = 15;
        x = cos(angle) * sign * hypotenuse;
        y = sin(angle) * sign * hypotenuse;
    }

    void applyVector(Sint16 *ox, Sint16 *oy) const {
        *ox += static_cast<int>(std::round(x));
        *oy += static_cast<int>(std::round(y));
    }

    void bump(BumpType bumpType, PaddleDirection paddleDirection) {
        switch (bumpType) {
            case BumpType::BOTH:
                x = -x;
                y = -y;
                break;
            case BumpType::WALL:
                y = -y;
                break;
            case BumpType::PADDLE:
                x = -x;
                double angle = 0;
                if (paddleDirection != PaddleDirection::NOT_MOVING) {
                    double_t degrees = rand() % 500 / 100 + 15;
                    degrees *= paddleDirection == PaddleDirection::MOVING_UP ? -1 : 1;
                    angle = toRadians(degrees);

                    // Adjusting y direction based on the angle
                    y = sin(angle) * hypotenuse;
                }
        }
    }

};