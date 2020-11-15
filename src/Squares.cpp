#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups


class MyButton: public rose::Entity {
    public:
        MyButton() = default;
    private:
};


class MenuLayer: public rose::Layer{
    public:
        MenuLayer() {
            startButton = std::make_shared<MyButton>();
        }
        virtual ~MenuLayer() {
        }
        virtual void Update() override {
            glm::ivec2 mouse;
            switch(rose::Input::GetInput()) {
                case rose::InputType::LeftTap: 
                    mouse = rose::Input::GetMousePos();
                    std::cout << "Left tap at x: " << mouse.x << ", y: " << mouse.y << std::endl;
                    break;
                case rose::InputType::RightTap: 
                    mouse = rose::Input::GetMousePos();
                    std::cout << "Right tap at x: " << mouse.x << ", y: " << mouse.y << std::endl;
                    break;
                case rose::InputType::Close:
                    //Application::Quit();  //get instance of application and quit (eg, set m_Quit to true).  Look at todo.txt for singleton details
                    break;
            }

            /*
            glm::vec2 offscreen = {100.0f, 100.0f};
            glm::vec2 mouseCoords = Input::GetMouseCoords();
            if(Input::LeftPressed and startButton->PointCollision(mouseCoords)) startButton->MoveTo(offscreen);*/
        }
        virtual void Draw() override {
            //DrawSprite(startButton->GetSprite(), startButton->GetModelMatrix); //if I go with this, then Layer needs a handle to renderer...
            //DrawSprite(startButton);
        }
    private:
        std::shared_ptr<MyButton> startButton;
};



int main(int, char**) {
    //if we go with singleton route, this code will look like
    //std::unique_ptr<rose::Application> app = rose::Application::GetApplication();
    //std::shared_ptr<rose::Layer> layer = std::make_shared<MenuLayer>(); //this won't change
    //app->SetLayer(layer);
    //app->Run();
    //app->Shutdown();
    std::unique_ptr<rose::Application> squaresApp = std::make_unique<rose::Application>();
    std::shared_ptr<rose::Layer> layer = std::make_shared<MenuLayer>();

    squaresApp->SetLayer(layer);
    squaresApp->Run();
    squaresApp->Shutdown();

    return 0;
}


