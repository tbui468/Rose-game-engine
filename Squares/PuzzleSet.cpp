#include "PuzzleSet.h"
#include "Puzzle.h"
#include "PuzzleIcon.h"

namespace sqs {


    PuzzleSet::PuzzleSet(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
        Entity(sprite, size, boundingBox, pos) {
        }


    PuzzleSet::~PuzzleSet() {
        ClearAllData(); 
    }


    void PuzzleSet::Open() {
        Close(); //temp: just to make sure we don't open too many puzzles
        m_DestroyPuzzles = false;

        //create puzzles belong to this set
        rose::Sprite sprite = {{32, 32}, {32, 32}};
        glm::vec2 size = {32.0f, 32.0f};
        glm::vec4 box = {0.0f, 0.0f, 32.0f, 32.0f};
        for(int i = 0; i < 8; ++i) {
            m_PuzzleList.emplace_back(new Puzzle(sprite, size, box, glm::vec2(360.0f + Puzzle::GetSpacing() * i, 0.0f), i));
        }

        OpenPuzzle(0);

        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->MoveBy({-360.0f, 0.0f});
        }

        //create puzzle icons
        rose::Sprite iconsprite = {{32, 32}, {8, 8}};
        glm::vec2 iconsize = {8.0f, 8.0f};
        glm::vec4 iconbox = {0.0f, 0.0f, 8.0f, 8.0f};
        float margin = 24.0f;
        int iconCount = 8;
        float halfLength = (iconCount - 1) * margin / 2.0f;
        for(int i = 0; i < iconCount; ++i) {
            m_PuzzleIconList.emplace_back(new PuzzleIcon(iconsprite, iconsize, iconbox, glm::vec2(-halfLength + margin * i, 150.0f), i));
        }

        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) icon->MoveBy({0.0f, -30.0f});
        }
    }


    void PuzzleSet::OpenPuzzle(int index){
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle){
                if(puzzle->GetIndex() == index) puzzle->Open();
                else puzzle->Close();
            }
        }
    }


    void PuzzleSet::Close() {
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->MoveBy({480.0f, 0.0f});
        }

        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) icon->MoveBy({0.0f, 150.0f});
        }

        m_DestroyPuzzles = true;
    }

    void PuzzleSet::ClearAllData() {
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) {
                delete puzzle;
                puzzle = nullptr;
            }
        }

        m_PuzzleList.clear();

        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) {
                delete icon;
                icon = nullptr;
            }
        }

        m_PuzzleIconList.clear();
    }

    bool PuzzleSet::IsOpen() const {
        return m_PuzzleList.size() > 0;
    }

    void PuzzleSet::OnAnimationEnd() {
        Entity::OnAnimationEnd();
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->OnAnimationEnd();
        }

        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) icon->OnAnimationEnd();
        }

        if(m_DestroyPuzzles) {
            ClearAllData();
        }
    }

    void PuzzleSet::OnAnimationUpdate(float t) {
        Entity::OnAnimationUpdate(t);
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->OnAnimationUpdate(t);
        }
        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) icon->OnAnimationUpdate(t);
        }
    }

    void PuzzleSet::Draw() {
        Entity::Draw();
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->Draw();
        }
        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) icon->Draw();
        }
    }


}
