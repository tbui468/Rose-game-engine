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
            int GetIndex() const { return m_Index; }
            int GetSetIndex() const { return m_SetIndex; } 
            bool IsOpen() const { return m_IsOpen; }
            void Open();
            void Close();
            virtual void Draw() const override;
            virtual void MoveTo(const glm::vec2& pos) override;
            virtual void MoveBy(const glm::vec2& shift) override;
            virtual void OnAnimationEnd() override;
            virtual void OnAnimationUpdate(float t) override;
            const glm::ivec2& GetDimensions() const { return m_Dimensions; }
            int GetWidth() const { return m_Dimensions.x; }
            int GetHeight() const { return m_Dimensions.y; }
        public: 
            static float GetSpacing() { return s_Spacing; }
        public: //fractal utility functions
            FractalCorners FindFractalCorners(float mousex, float mousey) const;
            Fractal* GetClosestFractal(float mousex, float mousey) const;
            Fractal* GetFractalContaining(const glm::ivec2& index) const;
            Fractal* GetFractalWithIndex(const glm::ivec2& index) const;
            const std::vector<Fractal*>& GetFractals() const { return m_Fractals; }
            std::vector<Fractal*>::iterator GetFractalIterator(Fractal* fractal);
        public: //fractal transformations
            std::vector<Fractal*> SplitOverlappingWith(FractalData fractalData);
            std::vector<Fractal*> SplitFractal(Fractal* fractal, const std::vector<FractalData>& fractalData);
            void MergeFractals(FractalData data);
            void SwapFractals(Fractal* fractalA, Fractal* fractalB);
            void RotateFractalCW(Fractal* fractal);
            void RotateFractalCCW(Fractal* fractal);
            void ReflectFractalX(Fractal* fractal);
            void ReflectFractalY(Fractal* fractal);
            TransformationData PeekLastTransformation() const { return m_TransformationStack.back(); }
            void UndoLastTransformation();
            int GetMaxTransformations() const { return m_MaxTransformations; }
            int GetTransformationCount() const { return m_TransformationStack.size(); }
        private:
            const int m_Index;
            const int m_SetIndex;
            glm::ivec2 m_Dimensions;  //cached this for now
            std::vector<Fractal*> m_Fractals;
            //        FractalCorners m_FractalCorners {nullptr, nullptr, nullptr, nullptr}; //replace this with a list of lists called m_MergeList
            //std::vector<std::vector<Fractal*>> m_MergeLists; //CreateFromMergeList() should be called in OnAnimationEnd() and then the list cleared
            std::vector<FractalData> m_MergeList; //just put in data of fractal to create at end of merge, iterate through fractals and destroy those contained inside 
            bool m_IsOpen {false};
            int m_MaxTransformations {0}; //* move to g_Data
            std::vector<TransformationData> m_TransformationStack; //* move to g_Data
            std::vector<UndoIcon*> m_UndoIcons;
        private:
            inline static const float s_Spacing {240.0f};
            inline static const float s_InitOffset {360.0f};
            inline static const rose::Sprite s_Sprite {{32, 32}, {32, 32}, rose::TextureType::Default};
            inline static const glm::vec2 s_ObjectSize {32.0f, 32.0f};
            inline static const glm::vec4 s_BoundingBox {0.0f, 0.0f, 32.0f, 32.0f};
    };

}

#endif //PUZZLE_H
