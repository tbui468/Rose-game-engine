#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups


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

class Puzzle: public rose::Entity {
    public:
        virtual ~Puzzle() {}
        Puzzle(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
            Entity(sprite, size, boundingBox, pos) {}
    private:
};


class PuzzleSet: public rose::Entity {
    public:
        virtual ~PuzzleSet() {}
        PuzzleSet(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
            Entity(sprite, size, boundingBox, pos) {
            }

        void Open() {
            Close(); //temp: just to make sure we don't open too many puzzles

            rose::Sprite sprite = {{32, 32}, {32, 32}};
            glm::vec2 size = {32.0f, 32.0f};
            glm::vec4 box = {0.0f, 0.0f, 32.0f, 32.0f};
            for(int i = 0; i < 8; ++i) {
                m_PuzzleList.emplace_back(std::make_shared<Puzzle>(sprite, size, box, glm::vec2(x() + 48.0f * i - 128.0f, y() + 32.0f)));
            }
        }

        void DrawPuzzles(rose::Application* app) {
            for(const std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
                app->Draw(puzzle);
            }
        }

        void Close() {
            for(std::shared_ptr<Entity>& puzzle: m_PuzzleList) {
                if(!puzzle.get()) delete puzzle.get();
            }
            m_PuzzleList.clear();
        }

        bool IsOpen() const {
            return m_PuzzleList.size() > 0;
        }

    private:
        std::vector<std::shared_ptr<Entity>> m_PuzzleList;
};


class MyButton: public rose::Entity {
    public:
        virtual ~MyButton() {}
        MyButton(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
            Entity(sprite, size, boundingBox, pos) {}
    private:
};


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

            startButton = std::make_shared<MyButton>(startSprite, size, boundingBox, glm::vec2(0.0f, 32.0f));
            quitButton = std::make_shared<MyButton>(quitSprite, size, boundingBox, glm::vec2(0.0f, -32.0f));
            closeButton = std::make_shared<MyButton>(closeSprite, tinysize, tinyboundingBox, glm::vec2(m_RightEdge + 16.0f, m_TopEdge - 16.0f));
            m_PuzzleSet0 = std::make_shared<PuzzleSet>(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(-32.0f, m_TopEdge + 32.0f));
            m_PuzzleSet1 = std::make_shared<PuzzleSet>(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(0.0f, m_TopEdge + 32.0f));
            m_PuzzleSet2 = std::make_shared<PuzzleSet>(puzzleSetSprite, smallsize, smallboundingBox, glm::vec2(32.0f, m_TopEdge + 32.0f));

            m_App = rose::Application::GetApplication();
        }

        virtual ~MenuLayer() {}


        virtual void Update() override {
            /*//this would a nice API to script with (essentially hiding input processing and collision detection)
              for(Fractal& fractal: fractaList) {
              if(fractal->Flick(Left) && fractal->CanTranslate(Left)) {
              Swap(fractal, Left);
              }
              }*/
            //process inputs
            glm::ivec2 mouse = rose::Input::GetMousePos();
            switch(rose::Input::GetInput()) {
                case rose::InputType::LeftTap: 
                    if(quitButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
                        m_App->Quit();
                    if(startButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
                        startButton->MoveTo(glm::vec2(m_RightEdge + 96.0f, startButton->y()));
                        quitButton->MoveTo(glm::vec2(m_LeftEdge - 96.0f, quitButton->y()));
                        closeButton->MoveTo(glm::vec2(m_RightEdge - 16.0f, closeButton->y()));
                        m_PuzzleSet0->MoveTo(glm::vec2(m_PuzzleSet0->x(), 0.0f));
                        m_PuzzleSet1->MoveTo(glm::vec2(m_PuzzleSet1->x(), 0.0f));
                        m_PuzzleSet2->MoveTo(glm::vec2(m_PuzzleSet2->x(), 0.0f));
                        m_Parameter = 0.0f;
                        m_Start = true;
                    }
                    if(closeButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
                        if(m_PuzzleSet0->IsOpen()) {
                            m_PuzzleSet0->Close();
                            m_PuzzleSet0->MoveTo(glm::vec2(m_PuzzleSet0->x(), 0.0f));
                            m_PuzzleSet1->MoveTo(glm::vec2(m_PuzzleSet1->x(), 0.0f));
                            m_PuzzleSet2->MoveTo(glm::vec2(m_PuzzleSet2->x(), 0.0f));
                            m_Parameter = 0.0f;
                            m_Start = true;
                        }else{
                            startButton->MoveTo(glm::vec2(0.0f, startButton->y()));
                            quitButton->MoveTo(glm::vec2(0.0f, quitButton->y()));
                            closeButton->MoveTo(glm::vec2(m_RightEdge + 16.0f, closeButton->y()));
                            m_PuzzleSet0->MoveTo(glm::vec2(m_PuzzleSet0->x(), m_TopEdge + 32.0f));
                            m_PuzzleSet1->MoveTo(glm::vec2(m_PuzzleSet1->x(), m_TopEdge + 32.0f));
                            m_PuzzleSet2->MoveTo(glm::vec2(m_PuzzleSet2->x(), m_TopEdge + 32.0f));
                            m_Parameter = 0.0f;
                            m_Start = true;
                        }
                    }
                    if(m_PuzzleSet0->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
                        m_PuzzleSet0->Open();
                        m_PuzzleSet0->MoveTo(glm::vec2(m_PuzzleSet0->x(), m_TopEdge + 32.0f));
                        m_PuzzleSet1->MoveTo(glm::vec2(m_PuzzleSet1->x(), m_TopEdge + 32.0f));
                        m_PuzzleSet2->MoveTo(glm::vec2(m_PuzzleSet2->x(), m_TopEdge + 32.0f));
                        m_Parameter = 0.0f;
                        m_Start = true;
                    }
                    break;
                case rose::InputType::RightTap: 
                    std::cout << "X: " << mouse.x << " Y: " << mouse.y << std::endl;
                    break;
                case rose::InputType::Quit:
                    m_App->Quit();
                    break;
            }

            double deltaTime = m_App->GetDeltaTime();
            if(m_Parameter < 1.0f && m_Start) {
                m_Parameter += static_cast<float>(deltaTime) * .0015f;
                if(m_Parameter >= 1.0f) {
                    startButton->OnAnimationEnd();
                    quitButton->OnAnimationEnd();
                    closeButton->OnAnimationEnd();
                    m_PuzzleSet0->OnAnimationEnd();
                    m_PuzzleSet1->OnAnimationEnd();
                    m_PuzzleSet2->OnAnimationEnd();
                }else{
                    float sigmoid = Sigmoid(m_Parameter);
                    startButton->OnAnimationUpdate(sigmoid);
                    quitButton->OnAnimationUpdate(sigmoid);
                    closeButton->OnAnimationUpdate(sigmoid);
                    m_PuzzleSet0->OnAnimationUpdate(sigmoid);
                    m_PuzzleSet1->OnAnimationUpdate(sigmoid);
                    m_PuzzleSet2->OnAnimationUpdate(sigmoid);
                }
            }



        }


        virtual void Draw() override {
            m_App->Draw(quitButton);
            m_App->Draw(startButton);
            m_App->Draw(closeButton);
            m_App->Draw(m_PuzzleSet0);
            m_PuzzleSet0->DrawPuzzles(m_App);
            m_App->Draw(m_PuzzleSet1);
            m_App->Draw(m_PuzzleSet2);
            /*//This would be a nice overload for Draw, allowing arbitrary sprites and model matrices
              m_App->Draw(closeButton->GetSprite(), closeButton->GetModelMatrix());*/
        }


    private:
        std::shared_ptr<rose::Entity> quitButton;
        std::shared_ptr<rose::Entity> startButton;
        std::shared_ptr<rose::Entity> closeButton;
        std::shared_ptr<PuzzleSet> m_PuzzleSet0;
        std::shared_ptr<PuzzleSet> m_PuzzleSet1;
        std::shared_ptr<PuzzleSet> m_PuzzleSet2;
        std::vector<std::shared_ptr<PuzzleSet>> m_PuzzleSetList;
        rose::Application* m_App {nullptr};
        float m_Parameter {0.0f};
        bool m_Start {false};

        float m_LeftEdge {-240.0f};
        float m_RightEdge {240.0f};
        float m_TopEdge {135.0f};
        float m_BottomEdge {-135.0f};
};



int main(int, char**) {


    rose::Application* app = rose::Application::GetApplication();

    std::shared_ptr<rose::Layer> layer = std::make_shared<MenuLayer>(); 

    app->SetClearColor(glm::ivec3(253, 246, 227));
    app->SetLayer(layer);

    app->Run();
    app->Shutdown();

    return 0;
}


