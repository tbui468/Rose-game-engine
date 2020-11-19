#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "CommandCode.h"
#include "Input.h"

namespace rose {

    
    Application* Application::GetApplication() {
        if(!s_Application) {
            s_Application = new Application();
        }

        return s_Application;
    }

    std::shared_ptr<Renderer> Application::GetRenderer() {
        assert(s_Application);
        return m_Renderer;
    }

    void Application::Quit() {
        m_Quit = true;
    }

    Application::Application() {
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "SDL_Init error!!!" << SDL_GetError() << std::endl;
        }

        char* exePath = GetExecutablePath();
        std::string exePathString(exePath);

        std::cout << exePathString << std::endl;

        bool fullScreen = false;
        m_Window = std::make_shared<Window>(960, 540, fullScreen);

        bool vsync = false;
        m_Renderer = std::make_shared<Renderer>(m_Window, vsync, exePathString);

    }


    char* Application::GetExecutablePath() const {
        char* dataPath;
        char* basePath = SDL_GetBasePath();
        if(basePath) {
            dataPath = basePath;
        }else{
            dataPath = SDL_strdup("./");
        }
        SDL_free(basePath);
        return dataPath;
    }

    void Application::SetLayer(std::shared_ptr<Layer> layer) {
        m_Layer = layer;
    }


    void Application::SetClearColor(const glm::ivec3& color) {
        m_Renderer->SetClearColor(color);
    }

    void Application::Run() {


        //delta time
        m_Now = SDL_GetPerformanceCounter();
        m_Last = 0;
        m_DeltaTime = 0.0;


        while(!m_Quit) {
            m_Last = m_Now;
            m_Now = SDL_GetPerformanceCounter();
            m_DeltaTime = static_cast<double>((m_Now - m_Last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());


            m_Renderer->ClearQuads();
            m_Layer->Update();
            m_Layer->Draw();


            m_Renderer->DrawScene();

            SDL_GL_SwapWindow(m_Window->GetHandle());
        }

    }




    void Application::Shutdown() {
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


