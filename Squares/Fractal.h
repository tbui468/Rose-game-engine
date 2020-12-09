#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"
#include "Data.h"

namespace sqs {

    enum class OverlapType {
        None = 0, //
        Within, //proper subset
        Partial,
        Enclose, //fractal is larger and encloses other fractal
        Equal
    };


    class Fractal: public rose::Entity {
        public:
            Fractal(const std::vector<FractalElement>& elements, const glm::ivec2& index, const glm::vec2& coords, int puzzleIndex);
            virtual ~Fractal() {}
            int GetSize() const { return m_Size; }
            FractalElement GetSubElement(const glm::ivec2& index) const;
            glm::ivec2 GetIndex() const { return m_Index; }
            void SetIndex(const glm::ivec2& index) { m_Index = index; }
            bool Contains(const glm::ivec2& index) const; //todo: replace all calls to this with static version using fractal data
            void WriteData(std::vector<PuzzleSetData>& data, int setIndex, int puzzleIndex) const;
        public:
            virtual void RotateBy(float angle) override;
            virtual void ScaleTo(const glm::vec2& scale) override;
            virtual rose::Edge EdgeCollision(float pointX, float pointY) const override;
            virtual rose::Corner CornerCollision(float pointX, float pointY) const override;
            virtual void OnAnimationEnd() override;
        public:
            static rose::EntityData MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& fractalIndex, const glm::vec2& pos, int puzzleIndex);
            static void UpdateTextureData(const std::vector<FractalElement>& elements, const glm::ivec2& fractalIndex, int puzzleIndex);
            static glm::ivec2 GetTextureStart(const glm::ivec2& fractalIndex, int puzzleIndex);
            static glm::vec2 GetCoordsForTarget(const glm::ivec2& index, int size, const glm::ivec2& targetIndex, int targetSize, 
                                                const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static glm::vec2 GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static OverlapType FindOverlapType(FractalData f1, FractalData f2); //com
        private:
            Fractal(rose::EntityData e, const std::vector<FractalElement>& elements, const glm::ivec2& index, int puzzleIndex);
            int GetPuzzleIndex() const { return m_PuzzleIndex; }
            const std::vector<FractalElement>& GetElements() const { return m_Elements; }
            void SetElements(const std::vector<FractalElement>& elements) { m_Elements = elements; }
            void UpdateSprite();
            std::vector<FractalElement> GetSubElements(const glm::ivec2& index, int subElementSize) const;
        private:
            inline static const float s_UnitSize = 32.0f;
            inline static const float s_UnitMargin = 16.0f;
            glm::ivec2 m_Index {-1, -1};
            const int m_PuzzleIndex;
            int m_Size {0}; //this could be const too
            std::vector<FractalElement> m_Elements;
    };


}

#endif // FRACTAL_H
