#include "PuzzleIcon.h"
#include "Puzzle.h"

namespace sqs {

    PuzzleIcon::PuzzleIcon(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, Puzzle* puzzle):
            Entity(sprite, size, boundingBox, pos) {
                m_PuzzleHandle = puzzle;
            }

}
