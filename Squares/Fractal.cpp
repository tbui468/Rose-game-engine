#include "Fractal.h"
#include "Puzzle.h"

namespace sqs {

    Fractal::Fractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& pos) 
        : Fractal(MakeEntityData(elements, index, pos)){
            m_Size = floor(sqrt(elements.size() + 1));
            m_Index = index;
    }

    Fractal::Fractal(rose::EntityData e): Entity(e.sprite, e.size, e.boundingBox, e.position) {}


    //Temp: using right click as a flick left to test
    bool Fractal::LeftFlick(rose::InputType input, float startX, float startY) {
        if(input == rose::InputType::RightDown && PointCollision(startX, startY)) return true;
        return false;
    }

    rose::EntityData Fractal::MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& pos) {
        int size = floor(sqrt(elements.size() + 1));
        float fWidth = UnitSize() * size;
        float fHeight = UnitSize() * size;

        //texture stuff
        glm::ivec2 texStart = glm::ivec2(index.x * UnitSize(), index.y * UnitSize());
        rose::Sprite sprite = { texStart, {fWidth, fHeight}, rose::TextureType::Custom};

        std::shared_ptr<rose::Renderer> renderer = (rose::Application::GetApplication())->GetRenderer();

        std::vector<rose::SubTextureMapping> texMapping;


        for(int row = 0; row < size; ++row) {
            for(int col = 0; col < size; ++col) {
                texMapping.push_back({{texStart.x + col * UnitSize(), texStart.y + row * UnitSize()}, {0, 0}, {UnitSize(), UnitSize()}});
                //elements are drawn inside fractal frame, so start point is offset by 1 and side length is reduced by 2 in each dimension
                switch(elements.at(row * size + col)) {
                    case FractalElement::Red:
                        texMapping.push_back({{texStart.x + col * UnitSize() + 1, texStart.y + row * UnitSize() + 1}, 
                                             {UnitSize() + 1, 1}, {UnitSize() - 2, UnitSize() - 2}});
                        break;
                    case FractalElement::Blue:
                        texMapping.push_back({{texStart.x + col * UnitSize() + 1, texStart.y + row * UnitSize() + 1}, 
                                             {UnitSize() + 1, UnitSize() + 1}, {UnitSize() - 2, UnitSize() - 2}});
                        break;
                }
            }
        }

        renderer->SetCustomTexture(texMapping);


        glm::vec2 entitySize = glm::vec2(fWidth, fHeight);
        glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, fWidth, fHeight);
        rose::EntityData e;

        e.sprite = sprite;
        e.size = entitySize;
        e.boundingBox = boundingBox;
        e.position = pos;
        
        return e;
    }



    glm::vec2 Fractal::GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos) {
            float puzzleWidth = (puzzleDim.x - 1) * (UnitSize() + UnitMargin()); //this gives us width/height if all fractals are 1x1
            glm::vec2 topLeftGrid = glm::vec2(puzzlePos.x - puzzleWidth / 2.0f, puzzlePos.y + puzzleWidth/ 2.0f); //this gives center of topleft fractal
            glm::vec2 topLeftFractal = glm::vec2(topLeftGrid.x + index.x * (UnitSize() + UnitMargin()), 
                                                 topLeftGrid.y - index.y * (UnitSize() + UnitMargin()));
            float fractalWidth = (size - 1) * (UnitSize() + UnitMargin());
            return glm::vec2(topLeftFractal.x + fractalWidth / 2.0f, topLeftFractal.y - fractalWidth / 2.0f);
    }


    //give the index of a 1x1 fractals, find its coordinates IF it were a part of the target fractal (with the given index and size)
    glm::vec2 Fractal::GetCoordsForTarget(const glm::ivec2& index, const glm::ivec2& targetIndex, int targetSize, 
                                          const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos) {

            glm::vec2 targetCenter = GetCoords(targetIndex, targetSize, puzzleDim, puzzlePos);

            float halfWidth = UnitSize() * (targetSize - 1) * 0.5f;

            glm::vec2 targetTopLeft = glm::vec2(targetCenter.x - halfWidth, targetCenter.y + halfWidth);

            //using target center, start from targetIndex location and 
            glm::ivec2 relativeIndex = glm::ivec2(index.x - targetIndex.x, index.y - targetIndex.y);
            return glm::vec2(targetTopLeft.x + relativeIndex.x * UnitSize(), targetTopLeft.y - relativeIndex.y * UnitSize());
    }

}
