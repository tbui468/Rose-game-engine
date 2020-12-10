#ifndef PUZZLE_SET_H
#define PUZZLE_SET_H

#include "Rose.h"
#include "Puzzle.h"
#include "Data.h"

namespace sqs {

class PuzzleIcon;

class PuzzleSet: public rose::Entity {
    public:
        PuzzleSet(int index, const glm::vec2& pos);
        virtual ~PuzzleSet();
        void Open();
        void OpenPuzzle(Puzzle* puzzle);
        void Close();
        bool IsOpen() const;
        void ClearAllData();
        virtual void OnAnimationEnd() override;
        virtual void OnAnimationUpdate(float t) override;
        virtual void Draw() const override;
        const std::vector<Puzzle*>& GetPuzzles() const { return m_PuzzleList; }
        const std::vector<PuzzleIcon*>& GetPuzzleIcons() const { return m_PuzzleIconList; }
        int GetIndex() const { return m_Index; }
    private:
        std::vector<Puzzle*> m_PuzzleList;
        std::vector<PuzzleIcon*> m_PuzzleIconList;
        bool m_DestroyPuzzles {false};
        const int m_Index;
    private:
        inline static const rose::Sprite s_Sprite {{0, 0}, {32, 32}, rose::TextureType::Default};
        inline static const glm::vec2 s_ObjectSize {32.0f, 32.0f};
        inline static const glm::vec4 s_BoundingBox {0.0f, 0.0f, 32.0f, 32.0f};
};

}

#endif //PUZZLE_SET_H
