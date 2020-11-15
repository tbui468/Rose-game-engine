#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups

/*
class Button: public rose::Entity {
    public:
    private:
};*/

//should create and have buttons animate inwards, so that
//we can destroy the menu layer when player is playing puzzles, and 
//then recreate it when player exits puzzles back to main menu
class MenuLayer: public rose::Layer{
    public:
        MenuLayer() {
   //         startButton = rose::MakeRef<Button>();
        }
        virtual ~MenuLayer() {
        }
        virtual void Update() override {
            /*
            glm::vec2 offscreen = {100.0f, 100.0f};
            glm::vec2 mouseCoords = Input::GetMouseCoords();
            if(Input::LeftPressed and startButton->PointCollision(mouseCoords)) startButton->MoveTo(offscreen);*/
        }
        virtual void Draw() override {
            //Renderer::Draw(startButton->GetSprite(), startButton->GetModelMatrix);
        }
    private:
        //rose::Ref<Button> startButton;
};



int main(int, char**) {
    std::unique_ptr<rose::Application> squaresApp = std::make_unique<rose::Application>();
    std::shared_ptr<rose::Layer> layer = std::make_shared<MenuLayer>();

    squaresApp->SetLayer(layer);
    squaresApp->Run();
    squaresApp->Shutdown();

    return 0;
}


