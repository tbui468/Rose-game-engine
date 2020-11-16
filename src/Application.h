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
        void SetClearColor(const glm::ivec3& color);
        static Application* GetApplication();
    public: //rendering
//        void Draw(const std::string& spriteName, const glm::mat4& model); 
        void Draw(const std::shared_ptr<Entity>& entity);
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
