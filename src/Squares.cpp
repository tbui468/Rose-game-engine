#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups

class PuzzleSet: public rose::Entity {
    public:
    private:
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

            glm::vec2 size = glm::vec2(64.0f, 32.0f);
            glm::vec2 smallsize = glm::vec2(32.0f, 32.0f);
            glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, 64.0f, 32.0f);
            glm::vec4 smallboundingBox = glm::vec4(0.0f, 0.0f, 32.0f, 32.0f);

            startButton = std::make_shared<MyButton>(startSprite, size, boundingBox, glm::vec2(0.0f, 32.0f));
            quitButton = std::make_shared<MyButton>(quitSprite, size, boundingBox, glm::vec2(0.0f, -32.0f));
            closeButton = std::make_shared<MyButton>(closeSprite, smallsize, smallboundingBox, glm::vec2(m_RightEdge + 32.0f, m_TopEdge - 32.0f));

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
                        closeButton->MoveTo(glm::vec2(m_RightEdge - 32.0f, closeButton->y()));
                        m_Parameter = 0.0f;
                        m_Start = true;
                    }
                    if(closeButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
                        startButton->MoveTo(glm::vec2(0.0f, startButton->y()));
                        quitButton->MoveTo(glm::vec2(0.0f, quitButton->y()));
                        closeButton->MoveTo(glm::vec2(m_RightEdge + 32.0f, closeButton->y()));
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
                }else{
                    float sigmoid = Sigmoid(m_Parameter);
                    startButton->OnAnimationUpdate(sigmoid);
                    quitButton->OnAnimationUpdate(sigmoid);
                    closeButton->OnAnimationUpdate(sigmoid);
                }
            }



        }


        virtual void Draw() override {
            m_App->Draw(quitButton);
            m_App->Draw(startButton);
            m_App->Draw(closeButton);
            /*//This would be a nice overload for Draw, allowing arbitrary sprites and model matrices
            m_App->Draw(closeButton->GetSprite(), closeButton->GetModelMatrix());*/
        }


    private:
        std::shared_ptr<rose::Entity> quitButton;
        std::shared_ptr<rose::Entity> startButton;
        std::shared_ptr<rose::Entity> closeButton;
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

    app->SetClearColor(glm::ivec3(200, 255, 200));
    app->SetLayer(layer);

    app->Run();
    app->Shutdown();

    return 0;
}


