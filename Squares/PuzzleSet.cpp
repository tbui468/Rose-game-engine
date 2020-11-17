#include "PuzzleSet.h"
#include "Puzzle.h"

namespace sqs {


    PuzzleSet::PuzzleSet(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
        Entity(sprite, size, boundingBox, pos) {
        }


    void PuzzleSet::Open() {
        Close(); //temp: just to make sure we don't open too many puzzles
        m_DestroyPuzzles = false;

        rose::Sprite sprite = {{32, 32}, {32, 32}};
        glm::vec2 size = {32.0f, 32.0f};
        glm::vec4 box = {0.0f, 0.0f, 32.0f, 32.0f};
        for(int i = 0; i < 8; ++i) {
            m_PuzzleList.emplace_back(std::make_shared<Puzzle>(sprite, size, box, glm::vec2(360.0f + 128.0f * i, 0.0f)));
        }
        MovePuzzles(glm::vec2(-360.0f, 0.0f));
    }

    void PuzzleSet::DrawPuzzles(rose::Application* app) {
        for(const std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
            app->Draw(puzzle);
        }
    }

    void PuzzleSet::MovePuzzles(const glm::vec2& shift) {
        for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
            puzzle->MoveTo({puzzle->x() + shift.x, puzzle->y() + shift.y});
        }
    }

    void PuzzleSet::Close() {
        MovePuzzles(glm::vec2(480.0f, 0.0f));
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
            for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
                if(!puzzle.get()) delete puzzle.get();
            }
            m_PuzzleList.clear();
        }
    }

    void PuzzleSet::OnAnimationUpdate(float t) {
        rose::Entity::OnAnimationUpdate(t);
        for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
            puzzle->OnAnimationUpdate(t);
        }
    }

}
