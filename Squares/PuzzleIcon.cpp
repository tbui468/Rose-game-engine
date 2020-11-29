#include "PuzzleIcon.h"
#include "Puzzle.h"

namespace sqs {


    PuzzleIcon::PuzzleIcon(Puzzle* puzzle, int puzzleCount) 
        : Entity(s_Sprite, s_ObjectSize, s_BoundingBox, glm::vec2(s_Margin * ((1 - puzzleCount) / 2.0f + puzzle->GetIndex()), 150.f)) {
            m_PuzzleHandle = puzzle;
    }

}
