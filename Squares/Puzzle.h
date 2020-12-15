#ifndef PUZZLE_H
#define PUZZLE_H

#include <array>
#include "Rose.h"
#include "Fractal.h"
#include "UndoIcon.h"
#include "Data.h"

namespace sqs {


    struct FractalCorners {
        Fractal* TopLeft {nullptr};
        Fractal* TopRight {nullptr};
        Fractal* BottomLeft {nullptr};
        Fractal* BottomRight {nullptr};
    };

    class Puzzle: public rose::Entity {
        public:
            Puzzle(int index, int setIndex);
            virtual ~Puzzle();
            bool IsOpen() const { return m_IsOpen; }
            void Open();
            void Close();
            virtual void Draw() const override;
            virtual void MoveTo(const glm::vec2& pos) override;
            virtual void MoveBy(const glm::vec2& shift) override;
            virtual void OnAnimationEnd() override;
            virtual void OnAnimationUpdate(float t) override;
        public: //fractal utility functions
            FractalCorners FindFractalCorners(float mousex, float mousey) const;
            Fractal* GetClosestFractal(float mousex, float mousey) const;
            Fractal* GetFractalContaining(const glm::ivec2& index) const;
            Fractal* GetFractalWithIndex(const glm::ivec2& index) const;
            const std::vector<Fractal*>& GetFractals() const { return m_Fractals; }
            std::vector<Fractal*>::iterator GetFractalIterator(Fractal* fractal);
            void UpdateTextureData(FractalData data) const; //should only update custom texture corresponding to the fractal
        public: //fractal transformations
            std::vector<Fractal*> SplitOverlappingWith(FractalData fractalData);
            std::vector<Fractal*> SplitFractal(Fractal* fractal, const std::vector<FractalData>& fractalData);
            void Transform(FractalData data, TransformationType type); //@todo: make all the specific transformation functions private - only this one is public (const later)
            void MergeFractals(std::vector<Fractal*> fractals);
            inline void PushTransformation(TransformationData data) const { g_Data.at(m_SetIndex).puzzlesData.at(m_Index).transformationStack.push_back(data); }
            inline void PopTransformation() const { g_Data.at(m_SetIndex).puzzlesData.at(m_Index).transformationStack.pop_back(); }
            inline TransformationData PeekTransformation() const { return g_Data.at(m_SetIndex).puzzlesData.at(m_Index).transformationStack.back(); }
            void UndoLastTransformation();
            inline int32_t GetMaxTransformations() const { return g_Data.at(m_SetIndex).puzzlesData.at(m_Index).maxTransformations; }
            inline size_t GetTransformationCount() const { return g_Data.at(m_SetIndex).puzzlesData.at(m_Index).transformationStack.size(); }
            std::vector<FractalElement> GetElements(FractalData data) const; //@todo: rename to GetFractalElements
            FractalElement GetElementAt(int col, int row) const { return g_Data.at(m_SetIndex).puzzlesData.at(m_Index).elements.at(row * m_Dimensions.x + col); }
            void SetElementAt(int col, int row, FractalElement e) const { g_Data.at(m_SetIndex).puzzlesData.at(m_Index).elements.at(row * m_Dimensions.x + col) = e; }
        private:
            void SwapFractals(Fractal* fractalA, Fractal* fractalB);
            void RotateFractalCW(Fractal* fractal);
            void RotateFractalCCW(Fractal* fractal);
            void ReflectFractalX(Fractal* fractal);
            void ReflectFractalY(Fractal* fractal);
            static rose::Sprite CalculateSpriteData(FractalData data, int puzzleIndex);
        public:
            const int m_Index;
            const glm::ivec2 m_Dimensions;
            inline static const float s_Spacing {240.0f};
        private:
            const int m_SetIndex;
            std::vector<Fractal*> m_Fractals;
            std::vector<FractalData> m_MergeList; //just put in data of fractal to create at end of merge, iterate through fractals and destroy those contained inside 
            bool m_IsOpen {false};
            std::vector<UndoIcon*> m_UndoIcons;
        private:
            inline static const float s_InitOffset {360.0f};
            inline static const rose::Sprite s_Sprite {{32, 32}, {32, 32}, rose::TextureType::Default};
            inline static const glm::vec2 s_ObjectSize {32.0f, 32.0f};
            inline static const glm::vec4 s_BoundingBox {0.0f, 0.0f, 32.0f, 32.0f};
    };

}

#endif //PUZZLE_H
