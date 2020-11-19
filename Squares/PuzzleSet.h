#ifndef PUZZLE_SET_H
#define PUZZLE_SET_H

#include "Rose.h"

namespace sqs {

class PuzzleIcon;
class Puzzle;

class PuzzleSet: public rose::Entity {
    public:
        PuzzleSet(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos);
        virtual ~PuzzleSet();
        void Open();
        void OpenPuzzle(int index);
        void Close();
        bool IsOpen() const;
        void ClearAllData();
        virtual void OnAnimationEnd() override;
        virtual void OnAnimationUpdate(float t) override;
        virtual void Draw() const override;
        const std::vector<Puzzle*>& GetPuzzles() const { return m_PuzzleList; }
        const std::vector<PuzzleIcon*>& GetPuzzleIcons() const { return m_PuzzleIconList; }

    private:
        std::vector<Puzzle*> m_PuzzleList;
        std::vector<PuzzleIcon*> m_PuzzleIconList;
        bool m_DestroyPuzzles {false};
};

}

#endif //PUZZLE_SET_H
