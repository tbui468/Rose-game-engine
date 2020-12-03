#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "renderer/Texture.h"
#include "globals.h"
#include "InputType.h"


namespace rose {


    enum class Corner {
        None,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    enum class Edge {
        None,
        Left,
        Right,
        Top,
        Bottom
    };

    struct EntityData {
        Sprite sprite;
        glm::vec2 size;
        glm::vec4 boundingBox;
        glm::vec2 position;
    };


    class Entity {
        public:
            Entity(const Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
            virtual ~Entity() {}
        public:
            virtual void MoveTo(const glm::vec2& pos);
            virtual void MoveBy(const glm::vec2& shift);
            virtual void FadeTo(float alpha);
            virtual void ScaleTo(const glm::vec2& scale);
            virtual void RotateTo(float angle);
            virtual void RotateBy(float angle);
            virtual void OnAnimationUpdate(float t);
            virtual void OnAnimationEnd();
            virtual void Draw() const;
            virtual void SetSprite(const Sprite& sprite) { m_SpriteData = sprite; }
        public:
            virtual bool LeftTap(InputType input, float x, float y) const;
            virtual bool PointCollision(float pointX, float pointY) const;
            virtual Corner CornerCollision(float pointX, float pointY) const;
            virtual Edge EdgeCollision(float pointX, float pointY) const;
            virtual glm::mat4 GetModelMatrix() const;
            virtual const Sprite& GetSprite() const { return m_SpriteData; }

        public:
            virtual float x() const { return m_Pos.x / g_Scale; }
            virtual float y() const { return m_Pos.y / g_Scale; }

            //animation parameters
        private:
            glm::vec2 m_FromPos {0.0f, 0.0f};
            glm::vec2 m_ToPos {0.0f, 0.0f};


            float m_FromAlpha {1.0f};
            float m_Alpha {1.0f};
            float m_ToAlpha {1.0f};

        protected: //to allow fractal class to reset to 0.0f in OnAnimationEnd() to avoid saving angle/scale states
            glm::vec2 m_Pos {0.0f, 0.0f};

            float m_ToAngle {0.0f};
            float m_Angle {0.0f};
            float m_FromAngle {0.0f};

            glm::vec2 m_FromScale {1.0f, 1.0f};
            glm::vec2 m_Scale {1.0f, 1.0f};
            glm::vec2 m_ToScale {1.0f, 1.0f};
            glm::vec4 m_BoundingBox {0.0f, 0.0f, 0.0f, 0.0f};

        private: //these guys shouldn't change after being set
            Sprite m_SpriteData {{0, 0}, {0, 0}, TextureType::None};
            glm::vec2 m_Size {1.0f, 1.0f};
    };

}


#endif //ENTITY_H
