#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"

namespace sqs {

    class Puzzle;


    enum class FractalElement {
        Empty,
        Red,
        Blue,
        Green
    };
    
    class Fractal: public rose::Entity {
        public:
            Fractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& coords, int puzzleNumber);
            virtual ~Fractal() {}
            bool LeftFlick(rose::InputType input, float mousex, float mousey);
            glm::ivec2 GetIndex() const { return m_Index; }
            void SetIndex(const glm::ivec2& index) { m_Index = index; }
            int GetSize() const { return m_Size; }
            const std::vector<FractalElement>& GetElements() const { return m_Elements; }
            virtual void OnAnimationEnd() override;
            void UpdateSprite();
            int GetPuzzleNumber() const { return m_PuzzleNumber; };
            bool Contains(const glm::ivec2& index) const;
        public:
            static float UnitSize() { return s_UnitSize; }
            static float UnitMargin() { return s_UnitMargin; }
            static glm::vec2 GetCoordsForTarget(const glm::ivec2& unitindex, int size, const glm::ivec2& targetIndex, int targetSize, 
                                                const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static glm::vec2 GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
        private:
            Fractal(rose::EntityData e);
            static rose::EntityData MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& pos, int puzzleNumber);
            static void UpdateTextureData(const std::vector<FractalElement>& elements, const glm::ivec2& index, int puzzleNumber);
            static glm::ivec2 GetTextureStart(const glm::ivec2& index, int puzzleNumber);
            inline static float s_UnitSize = 32.0f;
            inline static float s_UnitMargin = 16.0f;
        private:
            glm::ivec2 m_Index {0, 0};
            int m_Size {0};
            std::vector<FractalElement> m_Elements;
            int m_PuzzleNumber {0};
    };

}


#endif //FRACTAL_H
