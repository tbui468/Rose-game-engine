#ifndef APPLICATION_H
#define APPLICATION_H

#include <array>

#include "Window.h"
#include "renderer/Renderer.h"
#include "Layer.h"
#include "Entity.h"
#include "Globals.h"
#include "Audio.h"

namespace rose {

enum MouseEvents {
    LeftButtonDown = 0,
    RightButtonDown,
    MiddleButtonDown, 
    LeftButtonUp,
    RightButtonUp,
    MiddleButtonUp, 
    WheelUp,
    WheelDown
};

class Application {
    public:
        virtual ~Application() {}
        void SetLayer(std::shared_ptr<Layer> layer);
        void Run();
        void Shutdown() const;
        void Quit();
        //InputType GetInput() const;
        bool PollInputs();
        glm::ivec2 GetMousePos() const;
        int32_t GetWindowWidth() const { return m_Window->GetWidth(); }
        int32_t GetWindowHeight() const { return m_Window->GetHeight(); }
        float GetProjWidth() const { return m_Renderer->GetProjWidth(); }
        float GetProjHeight() const { return m_Renderer->GetProjHeight(); }
        void SetClearColor(const glm::ivec3& color);
        double GetDeltaTime() const { return m_DeltaTime; }
        static Application* GetApplication();
        std::shared_ptr<Renderer> GetRenderer() const { return m_Renderer; }
        std::shared_ptr<Audio> GetAudio() const { return m_Audio; }
        char* GetExecutablePath() const;
    private:
        Application(); //making it private only because singleton for now
    private:
        bool m_Quit {false};
        std::shared_ptr<Window> m_Window {nullptr};
        std::shared_ptr<Renderer> m_Renderer {nullptr};
        std::shared_ptr<Audio> m_Audio {nullptr};
        std::shared_ptr<Layer> m_Layer {nullptr};
        inline static Application* s_Application {nullptr};
        double m_DeltaTime {0.0};
        uint64_t m_Now {0};
        uint64_t m_Last {0};
        std::array<bool, g_MaxKeys> m_Keys = {false};
        std::array<bool, g_MaxMouseButtons> m_Mouse = {false};
};

}


#endif //APPLICATION_H
