#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups

class PuzzleSet: public rose::Entity {
    public:
    private:
};

class MyButton: public rose::Entity {
    public:
        MyButton() = default;
        virtual ~MyButton() {}
        MyButton(const std::string& sprite, const glm::vec2& pos) : Entity(sprite, pos) {}
    private:
};


float Sigmoid(float _t) {
    return 1.0f / (1.0f + exp(-15.0f * (_t - 0.5f)));
}


class MenuLayer: public rose::Layer {
    public:
        MenuLayer() {
            startButton = std::make_shared<MyButton>("StartButton", glm::vec2(0.0f, 32.0f * rose::g_Scale));
            quitButton = std::make_shared<MyButton>("QuitButton", glm::vec2(0.0f, -32.0f * rose::g_Scale));
            closeButton = std::make_shared<MyButton>("CloseButton", glm::vec2(128.0f * rose::g_Scale, 0.0f));
            m_App = rose::Application::GetApplication();
        }

        virtual ~MenuLayer() {}


        virtual void Update() override {
            //process inputs
            glm::ivec2 mouse = rose::Input::GetMousePos();
            switch(rose::Input::GetInput()) {
                case rose::InputType::LeftTap: 
                    if(quitButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
                        m_App->Quit();
                    if(startButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
                        startButton->MoveTo(glm::vec2(128.0f * rose::g_Scale, startButton->y));
                        quitButton->MoveTo(glm::vec2(-128.0f * rose::g_Scale, quitButton->y));
                        closeButton->MoveTo(glm::vec2(96.0f * rose::g_Scale, closeButton->y));
                        m_Parameter = 0.0f;
                        m_Start = true;
                    }
                    if(closeButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
                        startButton->MoveTo(glm::vec2(0.0f, startButton->y));
                        quitButton->MoveTo(glm::vec2(0.0f, quitButton->y));
                        closeButton->MoveTo(glm::vec2(128.0f * rose::g_Scale, closeButton->y));
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
        }


    private:
        std::shared_ptr<rose::Entity> quitButton;
        std::shared_ptr<rose::Entity> startButton;
        std::shared_ptr<rose::Entity> closeButton;
        rose::Application* m_App {nullptr};
        float m_Parameter {0.0f};
        bool m_Start {false};
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


