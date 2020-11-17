#ifndef BUTTON_H
#define BUTTON_H

#include "Rose.h"

namespace sqs {


class Button: public rose::Entity {
    public:
        Button(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
        virtual ~Button() {}
        void GoAway();
        void ComeBack();
        void SetAnimationCoords(const glm::vec2& on, const glm::vec2& off);
    private:
        glm::vec2 m_OffScreenCoords;
        glm::vec2 m_OnScreenCoords;
};

}


#endif //BUTTON_H
