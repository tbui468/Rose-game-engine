#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "Renderer.h"
#include "Layer.h"

namespace rse {

class Application {
    public:
        Application() = default;
        virtual ~Application() {}
        void Init();
        void SetLayer(Layer* layer);
        void Run();
        void Shutdown();
    private:
        bool m_Quit = false;
        Window* m_Window;
        Renderer* m_Renderer;
        Layer* m_Layer;
    private: //delta time
        double m_DeltaTime;
        uint64_t m_Now = 0;
        uint64_t m_Last = 0;
};

}


#endif //APPLICATION_H
