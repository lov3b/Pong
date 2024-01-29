// SPDX-License-Identifier: BSD-2-Clause

#include "Vec2d.h"

Vec2d::Vec2d(float_t hypotenuse) : hypotenuse(hypotenuse) {
    std::random_device rd;
    mtRand = std::mt19937(rd());
    std::uniform_int_distribution<uint8_t> ints(0, 1);
    int sign = ints(mtRand) ? -1 : 1;

    std::uniform_real_distribution<float_t> angleGen(15, 75);
    double_t degrees = angleGen(mtRand);
    double_t angle = toRadians(degrees);
    x = sin(angle) * sign * hypotenuse;
    y = cos(angle) * sign * hypotenuse;

    smallAngleGen = std::uniform_real_distribution<double_t>(15, 20);
}

void Vec2d::applyVector(Sint16 *ox, Sint16 *oy) const {
    *ox += std::round(x);
    *oy += std::round(y);
}

void Vec2d::bump(BumpType bumpType, PaddleDirection paddleDirection) {
    // Make everything a bit faster so it's not boring
    hypotenuse *= bumpSpeedIncrease;
    x *= bumpSpeedIncrease;
    y *= bumpSpeedIncrease;

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
                double_t degrees = smallAngleGen(mtRand);
                degrees *= paddleDirection == PaddleDirection::MOVING_UP ? -1 : 1;
                angle = toRadians(degrees);

                // Adjusting y direction based on the angle
                y = sin(angle) * hypotenuse;
            }
    }
}
