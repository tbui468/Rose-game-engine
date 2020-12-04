#ifndef NEW_FRACTAL_H
#define NEW_FRACTAL_H

#include "Rose.h"

//this will be the fractal class

namespace sqs {


    enum class FractalElement;

    class NewFractal: public rose::Entity {
        public:
            NewFractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& coords, int puzzleIndex);
            virtual ~NewFractal() {}
            int GetSize() const { return m_Size; }
            int GetPuzzleIndex() const { return m_PuzzleIndex; }
            void SetIndex(const glm::ivec2& index) { m_Index = index; }
            glm::ivec2 GetIndex() const { return m_Index; }
            const std::vector<FractalElement>& GetElements() const { return m_Elements; }
            void SetElements(const std::vector<FractalElement>& elements) { m_Elements = elements; }
            void UpdateSprite();
            bool Contains(const glm::ivec2& index) const;
            std::vector<FractalElement> GetSubElements(const glm::ivec2& index, int size) const;
            FractalElement GetSubElement(const glm::ivec2& index) const;
            //override these guys next
            virtual void RotateBy(float angle) override;
            virtual void ScaleTo(const glm::vec2& scale) override;
            virtual rose::Edge EdgeCollision(float pointX, float pointY) const override;
            virtual rose::Corner CornerCollision(float pointX, float pointY) const override;
            virtual void OnAnimationEnd() override;
        private:
            NewFractal(rose::EntityData e);
        public:
            static rose::EntityData MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& fractalIndex, const glm::vec2& pos, int puzzleIndex);
            static void UpdateTextureData(const std::vector<FractalElement>& elements, const glm::ivec2& fractalIndex, int puzzleIndex);
            static glm::ivec2 GetTextureStart(const glm::ivec2& fractalIndex, int puzzleIndex);
            static glm::vec2 GetCoordsForTarget(const glm::ivec2& index, int size, const glm::ivec2& targetIndex, int targetSize, 
                                                const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static glm::vec2 GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static float UnitSize() { return s_UnitSize; }
            static float UnitMargin() { return s_UnitMargin; }
        private:
            inline static float s_UnitSize = 32.0f;
            inline static float s_UnitMargin = 16.0f;
        private:
            glm::ivec2 m_Index {-1, -1};
            int m_PuzzleIndex {-1};
            int m_Size {0};
            std::vector<FractalElement> m_Elements;
    };


}

#endif // NEW_FRACTAL_H
