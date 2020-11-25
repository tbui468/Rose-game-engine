#ifndef PUZZLE_H
#define PUZZLE_H

#include <array>
#include "Rose.h"
#include "Fractal.h"

namespace sqs {


struct FractalCorners {
    BaseFractal* TopLeft {nullptr};
    BaseFractal* TopRight {nullptr};
    BaseFractal* BottomLeft {nullptr};
    BaseFractal* BottomRight {nullptr};
};

class Puzzle: public rose::Entity {
    public:
        Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, int index);
        virtual ~Puzzle();
        int GetIndex() const { return m_Index; }
        bool IsOpen() const { return m_IsOpen; }
        void Open();
        void Close();
        void SplitFractal(BaseFractal* fractal);
        std::vector<BaseFractal*>::iterator GetFractalIterator(BaseFractal* fractal);
        virtual void Draw() const override;
        virtual void MoveTo(const glm::vec2& pos) override;
        virtual void MoveBy(const glm::vec2& shift) override;
        virtual void OnAnimationEnd() override;
        virtual void OnAnimationUpdate(float t) override;
        BaseFractal* GetFractal(const glm::ivec2& index) const;
        const std::vector<BaseFractal*>& GetFractals() const { return m_Fractals; }
        void SwapFractals(BaseFractal* fractalA, BaseFractal* fractalB);
        FractalCorners FindFractalCorners(float mousex, float mousey) const;
        BaseFractal* GetClosestFractal(float mousex, float mousey) const;
        void FormFractal(FractalCorners fc);
        const glm::ivec2& GetDimensions() const { return m_Dimensions; }
    private:
        int m_Index;
        std::vector<BaseFractal*> m_Fractals;
        FractalCorners m_FractalCorners {nullptr, nullptr, nullptr, nullptr};
        bool m_IsOpen {false};
        glm::ivec2 m_Dimensions {0, 0};

    //static members/functions
    public:
        static float GetSpacing() { return s_SPACING; }
    private:
        inline static float s_SPACING {240.0f};
};

}

#endif //PUZZLE_H
