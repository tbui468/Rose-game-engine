#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "renderer/Renderer.h"
#include "Layer.h"
#include "Entity.h"

namespace rose {

class Application {
    public:
        virtual ~Application() {}
        void SetLayer(std::shared_ptr<Layer> layer);
        void Run();
        void Shutdown();
        void Quit();
        int32_t GetWindowWidth() const { return m_Window->GetWidth(); }
        int32_t GetWindowHeight() const { return m_Window->GetHeight(); }
        float GetProjWidth() const { return m_Renderer->GetProjWidth(); }
        float GetProjHeight() const { return m_Renderer->GetProjHeight(); }
        void SetClearColor(const glm::ivec3& color);
        double GetDeltaTime() const { return m_DeltaTime; }
        static Application* GetApplication();
        std::shared_ptr<Renderer> GetRenderer();
    private:
        Application();
        bool m_Quit = false;
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<Layer> m_Layer;
    private: //delta time
        inline static Application* s_Application {nullptr};
        double m_DeltaTime;
        uint64_t m_Now = 0;
        uint64_t m_Last = 0;
};

}


#endif //APPLICATION_H
