#ifndef BASE_FRACTAL_H
#define BASE_FRACTAL_H

#include <glm/glm.hpp>
#include "Rose.h"

namespace sqs {

    class BaseFractal: public rose::Entity {
        public:
            BaseFractal(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, const glm::ivec2& index);
            virtual ~BaseFractal() {}
            bool LeftFlick(rose::InputType input, float mousex, float mousey);
            static glm::vec2 GetCoordsForTarget(const glm::ivec2& index, int size, const glm::ivec2& targetIndex, int targetSize, 
                                                const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);

            static glm::vec2 GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            glm::ivec2 GetIndex() const { return m_Index; }
            void SetIndex(const glm::ivec2& index) { m_Index = index; }
            static float UnitSize() { return s_UnitSize; }
            static float UnitMargin() { return s_UnitMargin; }
        private:
            inline static float s_UnitSize = 32.0f;
            inline static float s_UnitMargin = 16.0f;
            glm::ivec2 m_Index {0, 0};
    };

}


#endif //BASE_FRACTAL_H
