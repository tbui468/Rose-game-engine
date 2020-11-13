#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        virtual void SetTexture(CartCoords texCoords, uint32_t texWidth, uint32_t texHeight);
        virtual CartCoords GetTexCoords() const { return m_TexCoords; }
        virtual uint32_t GetTexWidth() const { return m_TexWidth; }
        virtual uint32_t GetTexHeight() const { return m_TexHeight; }
    public:
        virtual bool PointCollision(float pointX, float pointY) const;
        virtual void SetBoundingBox(float x, float y, float w, float h);
        virtual glm::mat4 GetModelMatrix() const;

    public:
        float x {0.0f};
        float y {0.0f};
        float xScale {1.0f};
        float yScale {1.0f};
        float angle {0.0f};
        float alpha {1.0f};
        uint32_t depth {0};

    //sprite data
    private: 
        CartCoords m_TexCoords;
        uint32_t m_TexWidth;
        uint32_t m_TexHeight;

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
        Rectangle m_BoundingBox; //x and y of bounding box are relative to x/y of entity, ege (0, 0) to center on entity center
};

}


#endif //ENTITY_H
