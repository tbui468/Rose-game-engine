#include "Fractal.h"
#include "Puzzle.h"

namespace sqs {



    Fractal::Fractal(int size, const glm::ivec2& index, const glm::ivec2& dim, const glm::vec2& puzzlePos) 
        : Fractal(MakeEntityData(size, index, dim, puzzlePos)) {
        m_Size = size;
        m_Index = index;
    }

    Fractal::Fractal(rose::EntityData e): Entity(e.sprite, e.size, e.boundingBox, e.position) {}


    //Temp: using right click as a flick left
    bool Fractal::LeftFlick(rose::InputType input, float startX, float startY) {
        if(input == rose::InputType::RightDown && PointCollision(startX, startY)) return true;
        return false;
    }


    void Fractal::SetIndex(const glm::ivec2& index) {
        m_Index = index;
    }


    rose::EntityData Fractal::MakeEntityData(int size, const glm::ivec2& index, const glm::ivec2& gridDim, const glm::vec2& puzzlePos) {
        float fractalUnitSize = 64.0f; //temp: just to test copying textures to a 2x2 fractal
        //rose::Sprite sprite = {{0, 0}, {fractalUnitSize * size, fractalUnitSize * size}, rose::TextureType::Custom};
        rose::Sprite sprite = {{0, 0}, {64, 64}, rose::TextureType::Custom};
        //needs to create a custom sprite for fractal using data from default texture
        //need to find fractal elements (eg, empty, red, blue, etc) and draw them 
        //for now, just draw them as emtpy
        //
        glm::vec2 entitySize = glm::vec2(fractalUnitSize * size, fractalUnitSize * size);
        glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, fractalUnitSize * size, fractalUnitSize * size);
        rose::EntityData e;
        float offset = (gridDim.x - 1)  * fractalUnitSize * .75f;
        glm::vec2 pos = glm::vec2(puzzlePos.x - offset + index.x * fractalUnitSize * 1.5f, puzzlePos.y + offset - index.y * fractalUnitSize * 1.5f);

        e.sprite = sprite;
        e.size = entitySize;
        e.boundingBox = boundingBox;
        e.position = pos;
        
        return e;
    }

}
