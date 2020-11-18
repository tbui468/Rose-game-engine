#include "PuzzleIcon.h"
#include "PuzzleSet.h"

namespace sqs {

    PuzzleIcon::PuzzleIcon(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, PuzzleSet* puzzleSet, int puzzleIndex):
            Entity(sprite, size, boundingBox, pos) {
                m_PuzzleSetHandle = puzzleSet;
                m_PuzzleIndex = puzzleIndex;
            }

    void PuzzleIcon::OnClick() {
        //temp: testing moving all puzzles down to test
        m_PuzzleSetHandle->MovePuzzlesBy(glm::vec2(0.0f, -100.0f));
    }
}
