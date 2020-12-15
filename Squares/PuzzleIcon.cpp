#include "PuzzleIcon.h"
#include "Puzzle.h"

namespace sqs {

    //temp: creating puzzleicon at 150.0f on the y axis so that it can animate onto the screen.   Actual world height is -135 to 135
    PuzzleIcon::PuzzleIcon(Puzzle* puzzle, int puzzleCount) 
        : Entity(s_Sprite, s_ObjectSize, s_BoundingBox, glm::vec2(s_Margin * ((1 - puzzleCount) / 2.0f + puzzle->m_Index), 150.f)) {
            m_PuzzleHandle = puzzle;
    }

}
