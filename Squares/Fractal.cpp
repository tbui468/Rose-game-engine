#include "Fractal.h"

namespace sqs {


    Fractal::Fractal(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, const glm::ivec2& index)
        :Entity(sprite, size, boundingBox, pos) {
            m_Index = index;
            m_Size = 1; //temp
    }

    //Temp: using right click as a flick left
    bool Fractal::LeftFlick(rose::InputType input, float startX, float startY) {
        if(input == rose::InputType::RightDown && PointCollision(startX, startY)) return true;
        return false;
    }


    void Fractal::SetIndex(const glm::ivec2& index) {
        m_Index = index;
    }

}
