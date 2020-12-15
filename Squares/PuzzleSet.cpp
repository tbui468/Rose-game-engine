#include "PuzzleSet.h"
#include "Puzzle.h"
#include "PuzzleIcon.h"

namespace sqs {


    PuzzleSet::PuzzleSet(int index, const glm::vec2& pos): Entity(s_Sprite, s_ObjectSize, s_BoundingBox, pos), m_Index(index) {}


    PuzzleSet::~PuzzleSet() {
        ClearAllData(); 
    }


    void PuzzleSet::Open() {
        Close(); //temp: just to make sure we don't open too many puzzles
        m_DestroyPuzzles = false;

        for(int i = 0; i < g_Data.at(m_Index).puzzlesData.size(); ++i) {
            m_PuzzleList.emplace_back(new Puzzle(i, m_Index));
        }


        OpenPuzzle(m_PuzzleList.at(0));

        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->MoveBy({-360.0f, 0.0f});
        }


        for(int i = 0; i < m_PuzzleList.size(); ++i) {
            m_PuzzleIconList.emplace_back(new PuzzleIcon(m_PuzzleList.at(i), m_PuzzleList.size()));
        }

        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) icon->MoveBy({0.0f, -30.0f});
        }
    }


    void PuzzleSet::OpenPuzzle(Puzzle* puzzleToOpen){
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle){
                //open/close appropriate puzzles
                if(&(*puzzleToOpen) == &(*puzzle)) puzzle->Open();
                else puzzle->Close();

                //move all puzzles
                float xPos = (puzzle->m_Index - puzzleToOpen->m_Index) * Puzzle::s_Spacing;
                puzzle->MoveTo(glm::vec2(xPos, 0.0f));
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

    void PuzzleSet::Draw() const {
        Entity::Draw();
        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->Draw();
        }
        for(PuzzleIcon* icon: m_PuzzleIconList) {
            if(icon) icon->Draw();
        }
    }


}
