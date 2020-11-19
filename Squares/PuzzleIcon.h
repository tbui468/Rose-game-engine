#ifndef PUZZLE_ICON_H
#define PUZZLE_ICON_H


#include "Rose.h"

namespace sqs {

class Puzzle;

class PuzzleIcon: public rose::Entity {
    public:
        PuzzleIcon(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, Puzzle* puzzle);
        virtual ~PuzzleIcon() {}
        void OnClick();
        Puzzle* GetPuzzle() const { return m_PuzzleHandle; }
    private:
        Puzzle* m_PuzzleHandle {nullptr};
};

}


#endif //PUZZLE_ICON_H
