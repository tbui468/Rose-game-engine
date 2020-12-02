#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups

#include "MenuLayer.h"


int main(int, char**) {
    rose::Application* app;

    {
        app = rose::Application::GetApplication();

        std::shared_ptr<rose::Layer> layer = std::make_shared<sqs::MenuLayer>(); 

        app->SetClearColor(glm::ivec3(253, 246, 227));
        app->SetLayer(layer);
    }

    app->Run();
    app->Shutdown();

    return 0;
}



