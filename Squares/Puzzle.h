#ifndef PUZZLE_H
#define PUZZLE_H

namespace sqs {

class Puzzle: public rose::Entity {
    public:
        Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
        virtual ~Puzzle() {}
    private:
};

}

#endif //PUZZLE_H
