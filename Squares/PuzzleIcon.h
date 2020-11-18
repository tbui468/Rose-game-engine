#ifndef PUZZLE_ICON_H
#define PUZZLE_ICON_H


#include "Rose.h"

namespace sqs {


class PuzzleIcon: public rose::Entity {
    public:
        PuzzleIcon(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, int puzzleIndex);
        virtual ~PuzzleIcon() {}
        void OnClick();
        int GetPuzzleIndex() const { return m_PuzzleIndex; }
    private:
        int m_PuzzleIndex {-1};
};

}


#endif //PUZZLE_ICON_H
