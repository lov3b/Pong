#include "Score.h"

Score::Score(SDL_Point *screenSize, uint8_t max_score) : MAX_SCORE(max_score), leftScore(0), rightScore(0),
                                                         TextScreen("", screenSize, std::make_optional(
                                                                 SDL_Point{screenSize->x / 2 - 50, 10})) {
}

void Score::update() {
    if (hasUpdated) return;

    std::stringstream ss;
    ss << std::to_string(leftScore) << " - " << std::to_string(rightScore);
    setText(ss.str());
    TextScreen::update();

    hasUpdated = true;
}

void Score::resetScore() {
    leftScore = rightScore = 0;
    sideWon_ = std::nullopt;
    hasUpdated = false;
}

void Score::incrementScore(const Side side) {
    hasUpdated = false;
    uint8_t incrementedScore;
    switch (side) {
        case Side::LEFT:
            incrementedScore = ++leftScore;
            break;
        case Side::RIGHT:
            incrementedScore = ++rightScore;
            break;
    }

    if (incrementedScore > MAX_SCORE)
        sideWon_ = side;
}