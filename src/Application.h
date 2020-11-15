#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "renderer/Renderer.h"
#include "Layer.h"

namespace rose {

class Application {
    public:
        Application();
        virtual ~Application() {}
        void SetLayer(std::shared_ptr<Layer> layer);
        void Run();
        void Shutdown();
        //static Application* GetApplication();
    private:
        bool m_Quit = false;
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<Layer> m_Layer;
    private: //delta time
        double m_DeltaTime;
        uint64_t m_Now = 0;
        uint64_t m_Last = 0;
};

}


#endif //APPLICATION_H
