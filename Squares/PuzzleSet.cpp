#include "PuzzleSet.h"
#include "Puzzle.h"
#include "PuzzleSelector.h"

namespace sqs {


    PuzzleSet::PuzzleSet(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
        Entity(sprite, size, boundingBox, pos) {
        }


    void PuzzleSet::Open() {
        Close(); //temp: just to make sure we don't open too many puzzles
        m_DestroyPuzzles = false;

        //create puzzles belong to this set
        rose::Sprite sprite = {{32, 32}, {32, 32}};
        glm::vec2 size = {32.0f, 32.0f};
        glm::vec4 box = {0.0f, 0.0f, 32.0f, 32.0f};
        for(int i = 0; i < 8; ++i) {
            m_PuzzleList.emplace_back(new Puzzle(sprite, size, box, glm::vec2(360.0f + 128.0f * i, 0.0f)));
        }
        MovePuzzlesBy(glm::vec2(-360.0f, 0.0f));

        //create puzzle selector belonging to this set
        rose::Sprite selectorSprite = {{32, 32}, {32, 32}};
        glm::vec2 selectorSize = {64.0f, 16.0f};
        glm::vec4 selectorBox = {0.0f, 0.0f, 64.0f, 16.0f};
        m_PuzzleSelector = new PuzzleSelector(selectorSprite, selectorSize, selectorBox, glm::vec2(0.0f, 150.0f), this);
        m_PuzzleSelector->MoveTo({m_PuzzleSelector->x(), 110.0f});
    }


    void PuzzleSet::MovePuzzlesBy(const glm::vec2& shift) {
        for(Entity* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->MoveTo({puzzle->x() + shift.x, puzzle->y() + shift.y});
        }
    }

    void PuzzleSet::Close() {
        MovePuzzlesBy(glm::vec2(480.0f, 0.0f));
        if(m_PuzzleSelector) m_PuzzleSelector->MoveTo({m_PuzzleSelector->x(), 150.0f});
        m_DestroyPuzzles = true;
    }

    bool PuzzleSet::IsOpen() const {
        return m_PuzzleList.size() > 0;
    }

    void PuzzleSet::OnAnimationEnd() {
        Entity::OnAnimationEnd();
        for(Entity* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->OnAnimationEnd();
        }

        if(m_PuzzleSelector) m_PuzzleSelector->OnAnimationEnd();

        if(m_DestroyPuzzles) {
            for(Entity* puzzle: m_PuzzleList) {
                if(puzzle) {
                    delete puzzle;
                    puzzle = nullptr;
                }
            }

            m_PuzzleList.clear();

            if(m_PuzzleSelector) {
                delete m_PuzzleSelector;
                m_PuzzleSelector = nullptr;
            }
        }
    }

    void PuzzleSet::OnAnimationUpdate(float t) {
        Entity::OnAnimationUpdate(t);
        for(Entity* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->OnAnimationUpdate(t);
        }
        if(m_PuzzleSelector) m_PuzzleSelector->OnAnimationUpdate(t);
    }

    void PuzzleSet::Draw() {
        Entity::Draw();
        for(Entity* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->Draw();
        }
        if(m_PuzzleSelector) m_PuzzleSelector->Draw();
    }


    bool PuzzleSet::ProcessIconTaps(rose::InputType input, float mousex, float mousey) {
        if(m_PuzzleSelector) return m_PuzzleSelector->ProcessIconTaps(input, mousex, mousey);
        else return false;
    }

}
