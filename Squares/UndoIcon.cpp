#include "UndoIcon.h"
#include "Puzzle.h"

namespace sqs {

    UndoIcon::UndoIcon(const glm::ivec2& pos, Puzzle* puzzle) 
        : Entity(s_Sprite, s_ObjectSize, s_BoundingBox, pos) {
            m_PuzzleHandle = puzzle;
    }

}
