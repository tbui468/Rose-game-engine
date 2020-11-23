#include "Fractal.h"
#include "Puzzle.h"

namespace sqs {



    Fractal::Fractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos) 
        : Fractal(MakeEntityData(elements, index, puzzleDim, puzzlePos)) {
        m_Size = floor(sqrt(elements.size() + 1));
        m_Index = index;
    }

    Fractal::Fractal(rose::EntityData e): Entity(e.sprite, e.size, e.boundingBox, e.position) {}


    //Temp: using right click as a flick left
    bool Fractal::LeftFlick(rose::InputType input, float startX, float startY) {
        if(input == rose::InputType::RightDown && PointCollision(startX, startY)) return true;
        return false;
    }



    rose::EntityData Fractal::MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::ivec2& gridDim, const glm::vec2& puzzlePos) {
        int size = floor(sqrt(elements.size() + 1));
        glm::ivec2 newIndex = {index.x / size, index.y/ size};
        float fractalUnitSize = 32.0f; //temp: just to test copying textures to a 2x2 fractal



        //texture stuff
        glm::ivec2 texStart = glm::ivec2(newIndex.x * fractalUnitSize * size, newIndex.y * fractalUnitSize * size);
        rose::Sprite sprite = { texStart, {fractalUnitSize * size, fractalUnitSize * size}, rose::TextureType::Custom};
       // rose::Sprite sprite = {{0, 0}, {64, 64}, rose::TextureType::Custom};

        std::shared_ptr<rose::Renderer> renderer = (rose::Application::GetApplication())->GetRenderer();

        std::vector<rose::SubTextureMapping> texMapping;


        for(int row = 0; row < size; ++row) {
            for(int col = 0; col < size; ++col) {
                texMapping.push_back({{texStart.x + col * fractalUnitSize, texStart.y + row * fractalUnitSize}, {0, 0}, {fractalUnitSize, fractalUnitSize}});
                switch(elements.at(row * size + col)) {
                    case FractalElement::Red:
                        texMapping.push_back({{texStart.x + col * fractalUnitSize + 1, texStart.y + row * fractalUnitSize + 1}, {33, 1}, {30, 30}});
                        break;
                    case FractalElement::Blue:
                        texMapping.push_back({{texStart.x + col * fractalUnitSize + 1, texStart.y + row * fractalUnitSize + 1}, {33, 33}, {30, 30}});
                        break;
                }
            }
        }

        renderer->SetCustomTexture(texMapping);


        glm::vec2 entitySize = glm::vec2(fractalUnitSize * size, fractalUnitSize * size);
        glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, fractalUnitSize * size, fractalUnitSize * size);
        rose::EntityData e;
        float offset = (gridDim.x/size - 1)  * size * fractalUnitSize * .75f;
        glm::vec2 pos = glm::vec2(puzzlePos.x - offset + newIndex.x * size * fractalUnitSize * 1.5f, puzzlePos.y + offset - newIndex.y * size * fractalUnitSize * 1.5f);

        e.sprite = sprite;
        e.size = entitySize;
        e.boundingBox = boundingBox;
        e.position = pos;
        
        return e;
    }

}
