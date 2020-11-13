#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include "Utility.h"

namespace rose {

class Entity {
    public:
        Entity() = default;
        Entity(CartCoords pos, uint32_t depth);
        virtual ~Entity() {}
        virtual void SetPosition(CartCoords coords);
        virtual void SetDepth(uint32_t depth);
    public:
        virtual void MoveTo(CartCoords pos);
        virtual void FadeTo(float alpha);
        virtual void ScaleTo(CartCoords scale);
        virtual void RotateTo(float angle);
        virtual void OnAnimationUpdate(float t);
        virtual void OnAnimationEnd();
    public:
        virtual bool PointCollision(float pointX, float pointY) const;
        virtual void SetBoundingBox(float x, float y, float w, float h);

    public:
        float x {0.0f};
        float y {0.0f};
        float xScale {1.0f};
        float yScale {1.0f};
        float angle {0.0f};
        float alpha {1.0f};
        uint32_t depth {0};

    //animation parameters
    private:
        CartCoords m_FromPos {0.0f, 0.0f};
        CartCoords m_ToPos {0.0f, 0.0f};

        Scale m_FromScale {1.0f, 1.0f};
        Scale m_ToScale {1.0f, 1.0f};

        float m_ToAngle {0.0f};
        float m_FromAngle {0.0f};

        float m_FromAlpha {1.0f};
        float m_ToAlpha {1.0f};

     //collisions
     private:
        //Quad m_Sprite;
        Rectangle m_BoundingBox; //x and y of bounding box are relative to x/y of entity, ege (0, 0) to center on entity center
};

}


#endif //ENTITY_H
