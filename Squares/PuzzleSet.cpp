#include "PuzzleSet.h"
#include "Puzzle.h"

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
            m_PuzzleList.emplace_back(std::make_shared<Puzzle>(sprite, size, box, glm::vec2(360.0f + 128.0f * i, 0.0f)));
        }
        MovePuzzles(glm::vec2(-360.0f, 0.0f));

        //create puzzle selector belonging to this set
        rose::Sprite selectorSprite = {{32, 32}, {32, 32}};
        glm::vec2 selectorSize = {64.0f, 16.0f};
        glm::vec4 selectorBox = {0.0f, 0.0f, 64.0f, 16.0f};
        m_PuzzleSelector = std::make_shared<PuzzleSelector>(selectorSprite, selectorSize, selectorBox, glm::vec2(0.0f, 150.0f));
        m_PuzzleSelector->MoveTo({m_PuzzleSelector->x(), 100.0f});
    }

    void PuzzleSet::DrawPuzzles(rose::Application* app) {
        for(const std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
            app->Draw(puzzle);
        }
        if(m_PuzzleSelector) app->Draw(m_PuzzleSelector);
    }

    void PuzzleSet::MovePuzzles(const glm::vec2& shift) {
        for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
            puzzle->MoveTo({puzzle->x() + shift.x, puzzle->y() + shift.y});
        }
    }

    void PuzzleSet::Close() {
        MovePuzzles(glm::vec2(480.0f, 0.0f));
        if(m_PuzzleSelector) m_PuzzleSelector->MoveTo({m_PuzzleSelector->x(), 150.0f});
        m_DestroyPuzzles = true;
    }

    bool PuzzleSet::IsOpen() const {
        return m_PuzzleList.size() > 0;
    }

    void PuzzleSet::OnAnimationEnd() {
        rose::Entity::OnAnimationEnd();
        for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
            puzzle->OnAnimationEnd();
        }

        if(m_DestroyPuzzles) {
            //destroy all puzzles
            for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
                if(!puzzle.get()) puzzle.reset();
            }
            m_PuzzleList.clear();

            //destroy puzzleselector
            if(m_PuzzleSelector) {
                m_PuzzleSelector.reset();
            }
        }
    }

    void PuzzleSet::OnAnimationUpdate(float t) {
        rose::Entity::OnAnimationUpdate(t);
        for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
            puzzle->OnAnimationUpdate(t);
        }
        if(m_PuzzleSelector) m_PuzzleSelector->OnAnimationUpdate(t);
    }

}
