#ifndef PUZZLE_SELECTOR_H
#define PUZZLE_SELECTOR_H

#include "Rose.h"

namespace sqs {

class PuzzleSet;
class PuzzleIcon;


class PuzzleSelector: public rose::Entity {
    public:
        PuzzleSelector(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos, PuzzleSet* puzzleSet);
        virtual ~PuzzleSelector();
        virtual void MoveTo(const glm::vec2& pos) override;
        virtual void OnAnimationEnd() override;
        virtual void OnAnimationUpdate(float t) override;
        virtual void Draw() override;
        bool ProcessIconTaps(rose::InputType input, float mousex, float mousey);
    private:
        std::vector<PuzzleIcon*> m_PuzzleIconList;
        PuzzleSet* m_PuzzleSetHandle {nullptr};
};


}


#endif //PUZZLE_SELECTOR_H
