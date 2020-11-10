#ifndef QUAD_H
#define QUAD_H

#include <stdint.h>
#include "SquaresCommon.h"

namespace sqs {

class Quad {
    public:
        Quad() = default;
        virtual ~Quad() {}
        void SetPosition(CartCoords coords);
        void SetDepth(uint32_t depth);
    public:
        void MoveTo(CartCoords pos);
        void FadeTo(float alpha);
        void ScaleTo(CartCoords scale);
        void RotateTo(float angle);
        virtual void OnAnimationUpdate(float t);
        virtual void OnAnimationEnd();
    public:
        bool PointCollision(int pointX, int pointY) const;
        void SetBoundingCoords(float x, float y, float w, float h);

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
        Rectangle m_BoundingBox;
};

}


#endif //QUAD_H
