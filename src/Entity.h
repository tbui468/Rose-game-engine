#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utility.h"
#include "renderer/Texture.h"

namespace rose {


class Entity {
    public:
        Entity() = default;
        Entity(const std::string& sprite, const glm::vec2& pos);
        virtual ~Entity() {}
        virtual void SetPosition(const glm::vec2& coords);
    public:
        virtual void MoveTo(const glm::vec2& pos);
        virtual void FadeTo(float alpha);
        virtual void ScaleTo(const glm::vec2& scale);
        virtual void RotateTo(float angle);
        virtual void OnAnimationUpdate(float t);
        virtual void OnAnimationEnd();
    public:
        virtual const std::string& GetSpriteName() const { return m_SpriteName; }
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

    //sprite data
    private: 
        std::string m_SpriteName;

    //animation parameters
    private:
        glm::vec2 m_FromPos {0.0f, 0.0f};
        glm::vec2 m_ToPos {0.0f, 0.0f};

        glm::vec2 m_FromScale {1.0f, 1.0f};
        glm::vec2 m_ToScale {1.0f, 1.0f};

        float m_ToAngle {0.0f};
        float m_FromAngle {0.0f};

        float m_FromAlpha {1.0f};
        float m_ToAlpha {1.0f};

     //collisions
     private:
        //x and y of bounding box are relative to x/y of entity, ege (0, 0) to center on entity center
        glm::vec4 m_BoundingBox {0.0f, 0.0f, 0.0f, 0.0f};
};

}


#endif //ENTITY_H
