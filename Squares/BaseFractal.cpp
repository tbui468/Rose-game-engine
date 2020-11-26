#include "BaseFractal.h"

namespace sqs {

    BaseFractal::BaseFractal(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, const glm::ivec2& index)
        :Entity(sprite, size, boundingBox, pos), m_Index(index) {}

    bool BaseFractal::LeftFlick(rose::InputType input, float mousex, float mousey) {
        if(input == rose::InputType::RightDown && PointCollision(mousex, mousey)) return true;
        return false;
    }

    glm::vec2 BaseFractal::GetCoordsForTarget(const glm::ivec2& index, int size, const glm::ivec2& targetIndex, int targetSize, 
            const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos) {
        glm::vec2 targetCenter = GetCoords(targetIndex, targetSize, puzzleDim, puzzlePos);

        float targetHalfWidth = size * UnitSize() * (targetSize / size - 1) * 0.5f; //distance between centers of the fractals on left/right edges (or top/bottom)

        glm::vec2 targetTopLeft = glm::vec2(targetCenter.x - targetHalfWidth, targetCenter.y + targetHalfWidth); //center of top left fractal

        //using target center, start from targetIndex location and 
        glm::ivec2 relativeIndex = glm::ivec2(index.x - targetIndex.x, index.y - targetIndex.y);
        return glm::vec2(targetTopLeft.x + relativeIndex.x * UnitSize(), targetTopLeft.y - relativeIndex.y * UnitSize());
    }

    glm::vec2 BaseFractal::GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos) {
        float puzzleWidth = (puzzleDim.x - 1) * (UnitSize() + UnitMargin()); //this gives us width/height if all fractals are 1x1
        float puzzleHeight = (puzzleDim.y - 1) * (UnitSize() + UnitMargin());
        glm::vec2 topLeftGrid = glm::vec2(puzzlePos.x - puzzleWidth / 2.0f, puzzlePos.y + puzzleHeight/ 2.0f); //this gives center of topleft fractal
        glm::vec2 topLeftFractal = glm::vec2(topLeftGrid.x + index.x * (UnitSize() + UnitMargin()), 
                topLeftGrid.y - index.y * (UnitSize() + UnitMargin()));
        float fractalWidth = (size - 1) * (UnitSize() + UnitMargin());
        return glm::vec2(topLeftFractal.x + fractalWidth / 2.0f, topLeftFractal.y - fractalWidth / 2.0f);
    }


}
