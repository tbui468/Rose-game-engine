#ifndef PUZZLE_SELECTOR_H
#define PUZZLE_SELECTOR_H

#include "Rose.h"
#include "PuzzleIcon.h"

namespace sqs {


class PuzzleSelector: public rose::Entity {
    public:
        PuzzleSelector(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
        virtual ~PuzzleSelector() {}
    private:
};


}


#endif //PUZZLE_SELECTOR_H
