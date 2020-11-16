#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups

class MyButton: public rose::Entity {
    public:
        MyButton() = default;
        MyButton(const std::string& sprite, const glm::vec2& pos) : Entity(sprite, pos) {}
    private:
};

class MenuLayer: public rose::Layer {
    public:
        MenuLayer() {
            quitButton = std::make_shared<MyButton>("QuitButton", glm::vec2(0.0f, -100.0f));
            startButton = std::make_shared<MyButton>("StartButton", glm::vec2(0.0f, 100.0f));
            m_App = rose::Application::GetApplication();
        }
        virtual ~MenuLayer() {
        }
        virtual void Update() override {
            glm::ivec2 mouse = rose::Input::GetMousePos();
            switch(rose::Input::GetInput()) {
                case rose::InputType::LeftTap: 
                    if(quitButton->PointCollision(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
                        m_App->Quit();
                    break;
                case rose::InputType::RightTap: 
                    break;
                case rose::InputType::Quit:
                    m_App->Quit();
                    break;
            }

        }
        virtual void Draw() override {
            m_App->Draw(quitButton);
            m_App->Draw(startButton);
        }
    private:
        std::shared_ptr<rose::Entity> quitButton;
        std::shared_ptr<rose::Entity> startButton;
        rose::Application* m_App {nullptr};
};



int main(int, char**) {

    rose::Application* app = rose::Application::GetApplication();
 
    std::shared_ptr<rose::Layer> layer = std::make_shared<MenuLayer>(); 
    app->SetClearColor(glm::ivec3(255, 255, 255));
    app->SetLayer(layer);
    app->Run();
    app->Shutdown();

    return 0;
}


