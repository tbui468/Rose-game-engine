#ifndef PUZZLE_SET_H
#define PUZZLE_SET_H

#include "Rose.h"
#include "PuzzleSelector.h"

namespace sqs {

class PuzzleSet: public rose::Entity {
    public:
        PuzzleSet(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
        virtual ~PuzzleSet() {}
        void Open();
        void MovePuzzlesBy(const glm::vec2& shift);
        void Close();
        bool IsOpen() const;
        virtual void OnAnimationEnd() override;
        virtual void OnAnimationUpdate(float t) override;
        virtual void Draw() override;

    private:
        std::vector<std::shared_ptr<Entity>> m_PuzzleList;
        std::shared_ptr<Entity> m_PuzzleSelector {nullptr};
        bool m_DestroyPuzzles {false};
};

}

#endif //PUZZLE_SET_H
