#ifndef PUZZLE_SET_H
#define PUZZLE_SET_H

#include "Rose.h"

namespace sqs {

class PuzzleSelector;

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
        bool ProcessIconTaps(rose::InputType input, float mousex, float mousey);

    private:
        std::vector<Entity*> m_PuzzleList;
        PuzzleSelector* m_PuzzleSelector {nullptr};
        bool m_DestroyPuzzles {false};
};

}

#endif //PUZZLE_SET_H
