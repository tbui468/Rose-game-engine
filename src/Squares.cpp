#include "Rose.h"
#include "glad/glad.h"

class MainLayer: public rse::Layer{
    public:
        MainLayer() {
            //initialize all game state/objects
        }
        virtual ~MainLayer() {}
        virtual void Update() override {

        }
        virtual void Draw() override {

        }
    private:
        //game objects/states
};



int main(int, char**) {
    rse::Application* squaresApp = new rse::Application;
    rse::Layer* layer = new MainLayer();
    squaresApp->Init();
    squaresApp->SetLayer(layer);
    squaresApp->Run();
    squaresApp->Shutdown();
    return 0;
}


