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
            Fractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            virtual ~Fractal() {}
            bool LeftFlick(rose::InputType input, float mousex, float mousey);
            glm::ivec2 GetIndex() const { return m_Index; }
            void SetIndex(const glm::ivec2& index) { m_Index = index; }
            int GetSize() const { return m_Size; }
        private:
            Fractal(rose::EntityData e);
            static rose::EntityData MakeEntityData(const std::vector<FractalElement>& elements, 
                                                   const glm::ivec2& index, const glm::ivec2& gridDim, const glm::vec2& puzzlePos);
        private:
            glm::ivec2 m_Index {0, 0};
            int m_Size {0};
            Puzzle* puzzleHandle {nullptr};
    };

}


#endif //FRACTAL_H
