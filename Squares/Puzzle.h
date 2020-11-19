#ifndef PUZZLE_H
#define PUZZLE_H

#include <array>
#include "Rose.h"

namespace sqs {

class Fractal;

class Puzzle: public rose::Entity {
    public:
        Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, int index);
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
    private:
        int m_Index;
        std::array<Fractal*, 4> m_FractalGrid;
        bool m_IsOpen {false};

    //static members/functions
    public:
        static float GetSpacing() { return s_SPACING; }
    private:
        inline static float s_SPACING {240.0f};
};

}

#endif //PUZZLE_H
