#ifndef PUZZLE_FRACTAL_H
#define PUZZLE_FRACTAL_H

namespace sqs {

class PuzzleFractal: public Fractal {
    PuzzleFractal(rose::Sprite sprite, FractalData data, const glm::vec2& coords, CallbackData callback);
};

}

#endif //PUZZLE_FRACTAL_H
