#ifndef PUZZLE_H
#define PUZZLE_H

namespace sqs {

class Puzzle: public rose::Entity {
    public:
        Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, int index);
        virtual ~Puzzle() {}
        int GetIndex() const { return m_Index; }
        static float GetSpacing() { return s_SPACING; }
    private:
        int m_Index;
        inline static float s_SPACING {128.0f};
};

}

#endif //PUZZLE_H
