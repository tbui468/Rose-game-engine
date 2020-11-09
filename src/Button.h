#ifndef BUTTON_H
#define BUTTON_H

#include "Animated.h"
#include "BoundingBox.h"

namespace sqs {

class Button : public Animated {
    public:
        Button(float x, float y, float w, float h) {
            m_BoundingBox.SetBoundingCoords(x, y, w, h);
            m_Sprite.x = x;
            m_Sprite.y = y;
            m_Sprite.w = w;
            m_Sprite.h = h;
        }
        virtual ~Button() = default;
        //temp: just to test
        bool OnClick() {
            return true;   
        }

        bool Collision(float x, float y) {
            return m_BoundingBox.Collision(x, y);
        }
    private:
        //temp:
        Rectangle m_Sprite;
        BoundingBox m_BoundingBox; 
};

}

#endif //BUTTON_H
