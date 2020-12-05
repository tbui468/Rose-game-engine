#ifndef PUZZLE_H
#define PUZZLE_H

#include <array>
#include "Rose.h"
#include "Fractal.h"
#include "UndoIcon.h"

namespace sqs {

struct PuzzleData {
    int Width;
    int Height;
    std::vector<FractalElement> Elements;
};


struct FractalCorners {
    Fractal* TopLeft {nullptr};
    Fractal* TopRight {nullptr};
    Fractal* BottomLeft {nullptr};
    Fractal* BottomRight {nullptr};
};

enum class TransformationType {
    None,
    TranslatePosX,
    TranslateNegX,
    TranslatePosY,
    TranslateNegY,
    RotateCW,
    RotateCCW,
    ReflectX,
    ReflectY
};

struct TransformationData {
    TransformationType Transformation {TransformationType::None};
    glm::ivec2 FractalIndex {-1, -1};
    int FractalSize {0};
};

class Puzzle: public rose::Entity {
    public:
        Puzzle(FractalElement* elements, const glm::ivec2& dimension, int index);
        virtual ~Puzzle();
        int GetIndex() const { return m_Index; }
        bool IsOpen() const { return m_IsOpen; }
        void Open();
        void Close();
        virtual void Draw() const override;
        virtual void MoveTo(const glm::vec2& pos) override;
        virtual void MoveBy(const glm::vec2& shift) override;
        virtual void OnAnimationEnd() override;
        virtual void OnAnimationUpdate(float t) override;
        const glm::ivec2& GetDimensions() const { return m_Dimensions; }
    public:
        static float GetSpacing() { return s_SPACING; }
        static float GetInitOffset() { return s_InitOffset; }
        static rose::Sprite GetSprite() { return s_Sprite; }
        static glm::vec2 GetObjectSize() { return s_ObjectSize; }
        static glm::vec4 GetBoundingBox() { return s_BoundingBox; }
    public: //fractal utility functions
        FractalCorners FindFractalCorners(float mousex, float mousey) const;
        Fractal* GetClosestFractal(float mousex, float mousey) const;
        Fractal* GetFractal(const glm::ivec2& index) const;
        const std::vector<Fractal*>& GetFractals() const { return m_Fractals; }
        std::vector<Fractal*>::iterator GetFractalIterator(Fractal* fractal);
    public: //fractal transformations
        std::vector<Fractal*> SplitFractal(Fractal* fractal, const std::vector<FractalData>& fractalData);
        void MergeFractals(std::vector<Fractal*> mergeList);
        void SwapFractals(Fractal* fractalA, Fractal* fractalB);
        void RotateFractalCW(Fractal* fractal);
        void RotateFractalCCW(Fractal* fractal);
        void ReflectFractalX(Fractal* fractal);
        void ReflectFractalY(Fractal* fractal);
        void UndoTransformation();
        int GetMaxTransformations() const { return m_MaxTransformations; }
        int GetTransformationCount() const { return m_TransformationStack.size(); }
    private:
        Fractal* CreateFromMergeList(const std::vector<Fractal*>& mergeList);
    private:
        int m_Index;
        std::vector<Fractal*> m_Fractals;
//        FractalCorners m_FractalCorners {nullptr, nullptr, nullptr, nullptr}; //replace this with a list of lists called m_MergeList
        std::vector<std::vector<Fractal*>> m_MergeLists; //CreateFromMergeList() should be called in OnAnimationEnd() and then the list cleared
        bool m_IsOpen {false};
        glm::ivec2 m_Dimensions {0, 0};
        int m_MaxTransformations {0};
        std::vector<TransformationData> m_TransformationStack;
        std::vector<UndoIcon*> m_UndoIcons;
    private:
        inline static float s_SPACING {240.0f};
        inline static float s_InitOffset {360.0f};
        inline static rose::Sprite s_Sprite {{32, 32}, {32, 32}, rose::TextureType::Default};
        inline static glm::vec2 s_ObjectSize {32.0f, 32.0f};
        inline static glm::vec4 s_BoundingBox {0.0f, 0.0f, 32.0f, 32.0f};
};

}

#endif //PUZZLE_H
