#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include "SquaresCommon.h"

namespace sqs {

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
        float x;
        float y;
        float xScale;
        float yScale;
        float angle;
        float alpha;
        uint32_t depth;

    //animation parameters
    private:
        CartCoords m_FromPos;
        CartCoords m_ToPos;

        Scale m_FromScale;
        Scale m_ToScale;

        float m_ToAngle;
        float m_FromAngle;

        float m_FromAlpha;
        float m_ToAlpha;

     //collisions
     private:
        //Quad m_Sprite;
        Rectangle m_BoundingBox; //x and y of bounding box are relative to x/y of entity, ege (0, 0) to center on entity center
};

}


#endif //ENTITY_H
