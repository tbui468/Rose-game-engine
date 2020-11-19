#include "MenuLayer.h"
#include "Button.h"
#include "PuzzleSet.h"
#include "Puzzle.h"
#include "PuzzleIcon.h"
#include "Fractal.h"

namespace sqs {

    //@todo: move this function to a utility class, either in Squares or in Rose
    static float Sigmoid(float _t) {
        return 1.0f / (1.0f + exp(-15.0f * (_t - 0.5f)));
    }


    MenuLayer::MenuLayer(): Layer() {
        rose::Sprite startSprite = {{0, 96}, {64, 32}};
        rose::Sprite quitSprite = {{64, 0}, {64, 32}};
        rose::Sprite closeSprite = {{96, 96}, {32, 32}};
        rose::Sprite puzzleSetSprite = {{0, 0}, {32, 32}};

        glm::vec2 size = glm::vec2(64.0f, 32.0f);
        glm::vec2 smallsize = glm::vec2(32.0f, 32.0f);
        glm::vec2 tinysize = glm::vec2(16.0f, 16.0f);
        glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, 64.0f, 32.0f);
        glm::vec4 smallboundingBox = glm::vec4(0.0f, 0.0f, 32.0f, 32.0f);
        glm::vec4 tinyboundingBox = glm::vec4(0.0f, 0.0f, 16.0f, 16.0f);

        m_StartButton = new Button(startSprite, size, boundingBox, glm::vec2(0.0f, 32.0f));
        m_StartButton->SetAnimationCoords(glm::vec2(0.0f, 32.0f), glm::vec2(m_RightEdge + 96.0f, 32.0f));
        m_QuitButton = new Button(quitSprite, size, boundingBox, glm::vec2(0.0f, -32.0f));
        m_QuitButton->SetAnimationCoords(glm::vec2(0.0f, -32.0f), glm::vec2(m_LeftEdge - 96.0f, -32.0f));
        m_CloseButton = new Button(closeSprite, tinysize, tinyboundingBox, glm::vec2(m_RightEdge + 16.0f, m_TopEdge - 16.0f));
        m_CloseButton->SetAnimationCoords(glm::vec2(m_RightEdge - 16.0f, m_TopEdge - 16.0f), glm::vec2(m_RightEdge + 16.0f, m_TopEdge - 16.0f));


        m_PuzzleSet0 = new PuzzleSet(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(-32.0f, m_TopEdge + 32.0f));
        m_PuzzleSet1 = new PuzzleSet(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(0.0f, m_TopEdge + 32.0f));
        m_PuzzleSet2 = new PuzzleSet(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(32.0f, m_TopEdge + 32.0f));

        m_Entities.push_back((rose::Entity*)m_StartButton);
        m_Entities.push_back((rose::Entity*)m_QuitButton);
        m_Entities.push_back((rose::Entity*)m_CloseButton);
        m_Entities.push_back((rose::Entity*)m_PuzzleSet0);
        m_Entities.push_back((rose::Entity*)m_PuzzleSet1);
        m_Entities.push_back((rose::Entity*)m_PuzzleSet2);

        m_PuzzleSets.push_back(m_PuzzleSet0);
        m_PuzzleSets.push_back(m_PuzzleSet1);
        m_PuzzleSets.push_back(m_PuzzleSet2);

    }

    MenuLayer::~MenuLayer() {
        if(m_QuitButton) delete m_QuitButton;
        if(m_StartButton) delete m_StartButton;
        if(m_CloseButton) delete m_CloseButton;
        if(m_PuzzleSet0) delete m_PuzzleSet0;
        if(m_PuzzleSet1) delete m_PuzzleSet1;
        if(m_PuzzleSet2) delete m_PuzzleSet2;

        for(rose::Entity* e: m_Entities) delete e;
        for(PuzzleSet* ps: m_PuzzleSets) delete ps;
    }


    bool MenuLayer::Update(double deltaTime, rose::InputType input, const glm::ivec2& mousePos) {
        //get inputs - need to process them here and
        //then give processed inputs to Square entities
        glm::vec2 mouse = {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};

        if(m_Parameter <1.0f) input = rose::InputType::None; //temp: to avoid retting animations


        //process inputs - return true to quit application
        if(m_QuitButton->LeftTap(input, mouse.x, mouse.y) || input == rose::InputType::Quit) return true;
        if(input == rose::InputType::RightTap) {

        }

        if(m_StartButton->LeftTap(input, mouse.x, mouse.y)) {
            m_StartButton->GoAway();
            m_StartButton->ScaleTo(glm::vec2(2.0f, 1.0f));
            m_QuitButton->GoAway();
            m_QuitButton->ScaleTo(glm::vec2(1.0f, 2.0f));
            m_CloseButton->ComeBack();
            for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), 0.0f));
            m_Parameter = 0.0f;
            m_Start = true;
        }


        if(m_CloseButton->LeftTap(input, mouse.x, mouse.y)) {
            bool puzzleWasOpen = false;
            for(PuzzleSet* ps : m_PuzzleSets) {
                if(ps->IsOpen()) {
                    //need to make the puzzles MoveTo(....)
                    puzzleWasOpen = true;
                    ps->Close();
                    break;
                } 
            }

            if(puzzleWasOpen) {
                for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), 0.0f));
            }else{
                m_StartButton->ComeBack();
                m_StartButton->ScaleTo(glm::vec2(1.0f, 1.0f));
                m_QuitButton->ComeBack();
                m_QuitButton->ScaleTo(glm::vec2(1.0f, 1.0f));
                m_CloseButton->GoAway();
                for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), m_TopEdge + 32.0f));
            }

            m_Parameter = 0.0f;
            m_Start = true;
        }


        for(PuzzleSet* ps : m_PuzzleSets) {
            if(ps->LeftTap(input, mouse.x, mouse.y)) {
                ps->Open();
                for(PuzzleSet* psInside : m_PuzzleSets) psInside->MoveTo(glm::vec2(psInside->x(), m_TopEdge + 32.0f));
                m_Parameter = 0.0f;
                m_Start = true;
                break;
            }
            for(PuzzleIcon* icon: ps->GetPuzzleIcons()) {
                if(icon->LeftTap(input, mouse.x, mouse.y)) {
                    int index = icon->GetPuzzleIndex();
                    ps->OpenPuzzle(index);
                    for(Puzzle* puzzle: ps->GetPuzzles()) {
                        float xPos = (puzzle->GetIndex() - index) * Puzzle::GetSpacing();
                        puzzle->MoveTo(glm::vec2(xPos, 0.0f));
                    }
                    m_Parameter = 0.0f;
                    m_Start = true;
                    break;
                }
            }


        }

        if(Puzzle* puzzle = GetOpenPuzzle(m_PuzzleSets)) {
            //                std::cout << "I'm the active puzzle: " << puzzle->GetIndex() << std::endl;
            /*     for(Fractal* fractal: puzzle->GetFractals()) {
                   if(fractal->LeftFlick(input, mouse.x, mouse.y)) {
                   Fractal* other = puzzle->GetFractal(Left, fractal);
                   if(other) puzzle->Swap(fractal, other);
                   }
                   } */
        }



        if(m_Parameter < 1.0f && m_Start) {
            m_Parameter += static_cast<float>(deltaTime) * .0010f;
            if(m_Parameter >= 1.0f) {
                for(rose::Entity* e: m_Entities) e->OnAnimationEnd();
            }else{
                for(rose::Entity* e: m_Entities) e->OnAnimationUpdate(Sigmoid(m_Parameter));
            }
        }

        return false;
    }


    Puzzle* MenuLayer::GetOpenPuzzle(const std::vector<PuzzleSet*>& puzzleSets) {
        for(PuzzleSet* ps: puzzleSets) {
            if(ps) {
                if(ps->IsOpen()) {
                    for(Puzzle* puzzle: ps->GetPuzzles()) {
                        if(puzzle) if(puzzle->IsOpen()) return puzzle;
                    }
                }
            }
        }
        return nullptr;
    }


    void MenuLayer::Draw() {
        for(rose::Entity* e: m_Entities) {
            e->Draw();
        }
    }


}
