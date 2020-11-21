#include "Fractal.h"

namespace sqs {



    Fractal::Fractal(int size, const glm::ivec2& index, const glm::vec2& pos) : Fractal(MakeEntityData(size, index, pos)) {
        m_Index = index;
        m_Size = size; 
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


    rose::EntityData Fractal::MakeEntityData(int size, const glm::ivec2& index, const glm::vec2& pos) const {
        rose::Sprite sprite = {{0, 0}, {32, 32}, rose::TextureType::Custom};
        glm::vec2 entitySize = glm::vec2(32.0f, 32.0f);
        glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, 32.0f, 32.0f);
        rose::EntityData e;

        e.sprite = sprite;
        e.size = entitySize;
        e.boundingBox = boundingBox;
        e.position = pos;
        
        return e;
    }

}
