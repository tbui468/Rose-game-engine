#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups

#include "Puzzle.h"
#include "PuzzleSet.h"
#include "Button.h"

/*
class PuzzleIcon: public rose::Entity {
    public:
        virtual ~PuzzleIcon() {}
        PuzzleIcon(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
            Entity(sprite, size, boundingBox, pos) {}
    private:
};

class PuzzleIconSelector: public rose::Entity {
    public:
        virtual ~PuzzleIconSelector() {}
        PuzzleIconSelector(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
            Entity(sprite, size, boundingBox, pos) {
                for(int i = 0; i < 8; ++i) {
                    std::shared_ptr<PuzzleIcon> icon = std::make_shared<PuzzleIcon>(iconsprite, size, boundingbox, relativepos);
                    m_IconList.at(i) = icon;
                }
            }

         //mostly the same, but also moves all puzzleicons in m_IconList
         //might need to make this a friend class to access base class member variables
        virtual void OnAnimationUpdate(float _t) override {
            m_Pos.x = m_FromPos.x + (m_ToPos.x - m_FromPos.x) * _t;
            m_Pos.y = m_FromPos.y + (m_ToPos.y - m_FromPos.y) * _t;
            m_Scale.x = m_FromScale.x + (m_ToScale.x - m_FromScale.x) * _t;
            m_Scale.y = m_FromScale.y + (m_ToScale.y - m_FromScale.y) * _t;
            m_Angle = m_FromAngle + (m_ToAngle - m_FromAngle) * _t;
            m_Alpha = m_FromAlpha + (m_ToAlpha - m_FromAlpha) * _t;
            for(PuzzleIcon& p: m_IconList) {
                p->OnAnimationUpdate(_t);
            }
        }
    private:
        std::array<PuzzleIcon, 8> m_IconList;
};*/

namespace sqs {




float Sigmoid(float _t) {
    return 1.0f / (1.0f + exp(-15.0f * (_t - 0.5f)));
}


class MenuLayer: public rose::Layer {
    public:
        MenuLayer() {

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

            startButton = std::make_shared<Button>(startSprite, size, boundingBox, glm::vec2(0.0f, 32.0f));
            startButton->SetAnimationCoords(glm::vec2(0.0f, 32.0f), glm::vec2(m_RightEdge + 96.0f, 32.0f));
            quitButton = std::make_shared<Button>(quitSprite, size, boundingBox, glm::vec2(0.0f, -32.0f));
            quitButton->SetAnimationCoords(glm::vec2(0.0f, -32.0f), glm::vec2(m_LeftEdge - 96.0f, -32.0f));
            closeButton = std::make_shared<Button>(closeSprite, tinysize, tinyboundingBox, glm::vec2(m_RightEdge + 16.0f, m_TopEdge - 16.0f));
            closeButton->SetAnimationCoords(glm::vec2(m_RightEdge - 16.0f, m_TopEdge - 16.0f), glm::vec2(m_RightEdge + 16.0f, m_TopEdge - 16.0f));


            m_PuzzleSet0 = std::make_shared<PuzzleSet>(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(-32.0f, m_TopEdge + 32.0f));
            m_PuzzleSet1 = std::make_shared<PuzzleSet>(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(0.0f, m_TopEdge + 32.0f));
            m_PuzzleSet2 = std::make_shared<PuzzleSet>(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(32.0f, m_TopEdge + 32.0f));

            m_Entities.push_back(std::static_pointer_cast<rose::Entity>(startButton));
            m_Entities.push_back(std::static_pointer_cast<rose::Entity>(quitButton));
            m_Entities.push_back(std::static_pointer_cast<rose::Entity>(closeButton));
            m_Entities.push_back(std::static_pointer_cast<rose::Entity>(m_PuzzleSet0));
            m_Entities.push_back(std::static_pointer_cast<rose::Entity>(m_PuzzleSet1));
            m_Entities.push_back(std::static_pointer_cast<rose::Entity>(m_PuzzleSet2));

            m_PuzzleSets.push_back(m_PuzzleSet0);
            m_PuzzleSets.push_back(m_PuzzleSet1);
            m_PuzzleSets.push_back(m_PuzzleSet2);

            m_App = rose::Application::GetApplication();
        }

        virtual ~MenuLayer() {}


        virtual void Update() override {
            //get inputs
            glm::ivec2 mouseI = rose::Input::GetMousePos();
            glm::vec2 mouse = {static_cast<float>(mouseI.x), static_cast<float>(mouseI.y)};
            rose::InputType input = rose::Input::GetInput();


            //process inputs
            if(quitButton->LeftTap(input, mouse.x, mouse.y) || input == rose::InputType::Quit) m_App->Quit();
            if(input == rose::InputType::RightTap) {

            }

            if(startButton->LeftTap(input, mouse.x, mouse.y)) {
                startButton->GoAway();
                quitButton->GoAway();
                closeButton->ComeBack();
                for(std::shared_ptr<PuzzleSet>& ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), 0.0f));
                m_Parameter = 0.0f;
                m_Start = true;
            }

            if(closeButton->LeftTap(input, mouse.x, mouse.y)) {
                bool puzzleWasOpen = false;
                for(std::shared_ptr<PuzzleSet>& ps : m_PuzzleSets) {
                    if(ps->IsOpen()) {
                        //need to make the puzzles MoveTo(....)
                        puzzleWasOpen = true;
                        ps->Close();
                        break;
                    } 
                }

                if(puzzleWasOpen) {
                    for(std::shared_ptr<PuzzleSet>& ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), 0.0f));
                }else{
                    startButton->ComeBack();
                    quitButton->ComeBack();
                    closeButton->GoAway();
                    for(std::shared_ptr<PuzzleSet>& ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), m_TopEdge + 32.0f));
                }

                m_Parameter = 0.0f;
                m_Start = true;
            }


            for(std::shared_ptr<PuzzleSet>& ps : m_PuzzleSets) {
                if(ps->LeftTap(input, mouse.x, mouse.y)) {
                    ps->Open();
                    for(std::shared_ptr<PuzzleSet>& ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), m_TopEdge + 32.0f));
                    m_Parameter = 0.0f;
                    m_Start = true;
                    break;
                }
            }


            double deltaTime = m_App->GetDeltaTime();
            if(m_Parameter < 1.0f && m_Start) {
                m_Parameter += static_cast<float>(deltaTime) * .0015f;
                if(m_Parameter >= 1.0f) {
                    for(std::shared_ptr<rose::Entity>& e: m_Entities) e->OnAnimationEnd();
                }else{
                    for(std::shared_ptr<rose::Entity>& e: m_Entities) e->OnAnimationUpdate(Sigmoid(m_Parameter));
                }
            }



        }


        virtual void Draw() override {
            for(std::shared_ptr<rose::Entity>& e: m_Entities) {
                m_App->Draw(e);
//                e->Draw();
            }
            
            for(std::shared_ptr<PuzzleSet>& ps: m_PuzzleSets) {
                ps->DrawPuzzles(m_App);
            }
        }


    private:
        std::shared_ptr<Button> quitButton;
        std::shared_ptr<Button> startButton;
        std::shared_ptr<Button> closeButton;
        std::shared_ptr<PuzzleSet> m_PuzzleSet0;
        std::shared_ptr<PuzzleSet> m_PuzzleSet1;
        std::shared_ptr<PuzzleSet> m_PuzzleSet2;

        std::vector<std::shared_ptr<rose::Entity>> m_Entities;
        std::vector<std::shared_ptr<PuzzleSet>> m_PuzzleSets;
        std::vector<std::shared_ptr<Button>> m_Buttons;

        rose::Application* m_App {nullptr};
        float m_Parameter {0.0f};
        bool m_Start {false};

        float m_LeftEdge {-240.0f};
        float m_RightEdge {240.0f};
        float m_TopEdge {135.0f};
        float m_BottomEdge {-135.0f};
};

}

int main(int, char**) {


    rose::Application* app = rose::Application::GetApplication();

    std::shared_ptr<rose::Layer> layer = std::make_shared<sqs::MenuLayer>(); 

    app->SetClearColor(glm::ivec3(253, 246, 227));
    app->SetLayer(layer);

    app->Run();
    app->Shutdown();

    return 0;
}



