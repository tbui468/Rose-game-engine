#ifndef PUZZLE_SET_H
#define PUZZLE_SET_H

#include "Rose.h"

namespace sqs {

class PuzzleSet: public rose::Entity {
    public:
        PuzzleSet(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
        virtual ~PuzzleSet() {}
        void Open();
        void DrawPuzzles(rose::Application* app);
        void MovePuzzles(const glm::vec2& shift);
        void Close();
        bool IsOpen() const;
        virtual void OnAnimationEnd() override;
        virtual void OnAnimationUpdate(float t) override;

    private:
        std::vector<std::shared_ptr<Entity>> m_PuzzleList;
        bool m_DestroyPuzzles { false };
};

}

#endif //PUZZLE_SET_H
