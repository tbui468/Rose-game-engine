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
        const rose::Sprite sprite = {{32, 32}, {32, 32}};
        const glm::vec2 size = {32.0f, 32.0f};
        const glm::vec4 box = {0.0f, 0.0f, 32.0f, 32.0f};
        for(int i = 0; i < 8; ++i) {
            m_PuzzleList.emplace_back(new Puzzle(sprite, size, box, glm::vec2(360.0f + Puzzle::GetSpacing() * i, 0.0f), i));
        }

        OpenPuzzle(m_PuzzleList.at(0));

        for(Puzzle* puzzle: m_PuzzleList) {
            if(puzzle) puzzle->MoveBy({-360.0f, 0.0f});
        }

        //create puzzle icons
        const rose::Sprite iconsprite = {{32, 32}, {8, 8}};
        const glm::vec2 iconsize = {8.0f, 8.0f};
        const glm::vec4 iconbox = {0.0f, 0.0f, 8.0f, 8.0f};
        const float margin = 24.0f;
        const int iconCount = 8;
        const float halfLength = (iconCount - 1) * margin / 2.0f;

        for(int i = 0; i < iconCount; ++i) {
            m_PuzzleIconList.emplace_back(new PuzzleIcon(iconsprite, iconsize, iconbox, glm::vec2(-halfLength + margin * i, 150.0f), m_PuzzleList.at(i)));
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
                float xPos = (puzzle->GetIndex() - puzzleToOpen->GetIndex()) * Puzzle::GetSpacing();
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

    //////////static functions and variables
    std::vector<PuzzleSet*> PuzzleSet::s_PuzzleSets;

    void PuzzleSet::CreateSets() {
        const rose::Sprite puzzleSetSprite = {{0, 0}, {32, 32}};
        const glm::vec2 size = glm::vec2(32.0f, 32.0f);
        const glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, 32.0f, 32.0f);
        float topEdge = 135.0f;

        PuzzleSet::s_PuzzleSets.emplace_back(new PuzzleSet(puzzleSetSprite, size, boundingBox, glm::vec2(-32.0f, topEdge + 32.0f)));
        PuzzleSet::s_PuzzleSets.emplace_back(new PuzzleSet(puzzleSetSprite, size, boundingBox, glm::vec2(0.0f, topEdge + 32.0f)));
        PuzzleSet::s_PuzzleSets.emplace_back(new PuzzleSet(puzzleSetSprite, size, boundingBox, glm::vec2(32.0f, topEdge + 32.0f)));
    }

    std::vector<PuzzleSet*>& PuzzleSet::GetSets() {
        if(PuzzleSet::s_PuzzleSets.empty()) PuzzleSet::CreateSets();
        return PuzzleSet::s_PuzzleSets;
    }


}
