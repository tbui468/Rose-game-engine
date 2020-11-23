#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"

namespace sqs {

    class Puzzle;

    enum class FractalElement {
        Empty,
        Red,
        Blue
    };

    class Fractal: public rose::Entity {
        public:
            Fractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& coords);
            virtual ~Fractal() {}
            bool LeftFlick(rose::InputType input, float mousex, float mousey);
            glm::ivec2 GetIndex() const { return m_Index; }
            void SetIndex(const glm::ivec2& index) { m_Index = index; }
            int GetSize() const { return m_Size; }
            static float UnitSize() { return s_UnitSize; }
            static float UnitMargin() { return s_UnitMargin; }
            static glm::vec2 GetCoordsForTarget(const glm::ivec2& unitindex, const glm::ivec2& targetIndex, int targetSize, 
                                                const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static glm::vec2 GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
        private:
            Fractal(rose::EntityData e);
            static rose::EntityData MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& pos);
            inline static float s_UnitSize = 32.0f;
            inline static float s_UnitMargin = 16.0f;
        private:
            glm::ivec2 m_Index {0, 0};
            int m_Size {0};
            Puzzle* puzzleHandle {nullptr};
    };

}


#endif //FRACTAL_H
