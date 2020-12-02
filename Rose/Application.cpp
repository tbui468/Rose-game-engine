#include <vector>

#include <SDL.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "CommandCode.h"
#include "Instrumentor.h"


//puttint FMOD headers before glad.h triggers APIENTRY redefinition warning
#include <glad/glad.h>

namespace rose {


    Application* Application::GetApplication() {
        if(!s_Application) {
            s_Application = new Application();
        }

        return s_Application;
    }


    void Application::Quit() {
        m_Quit = true;
    }


    Application::Application() {
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "SDL_Init error!!!" << SDL_GetError() << std::endl;
        }


        m_Audio = std::make_shared<Audio>();

        bool fullScreen = false;
        m_Window = std::make_shared<Window>(960, 540, fullScreen);

        bool vsync = false;
        m_Renderer = std::make_shared<Renderer>(m_Window, vsync, GetExecutablePath());

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

        m_Now = SDL_GetPerformanceCounter();
        m_Last = 0;
        m_DeltaTime = 0.0;

        while(!m_Quit) {
            {
//                InstrumentationTimer timer("update");

                m_Last = m_Now;
                m_Now = SDL_GetPerformanceCounter();
                m_DeltaTime = static_cast<double>((m_Now - m_Last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());

                if(PollInputs()) break;

                glm::ivec2 mousei = GetMousePos();

                if(m_Layer->Update(m_DeltaTime, m_Keys, m_Mouse, glm::vec2(static_cast<float>(mousei.x), static_cast<float>(mousei.y)))) break;
            }

            {
 //               InstrumentationTimer timer("draw");
                m_Layer->Draw();
                m_Renderer->DrawScene();
                SDL_GL_SwapWindow(m_Window->GetHandle());
            }
        }

    }

    bool Application::PollInputs() {
        for(int i = 0; i < m_Keys.size(); ++i) {
            m_Keys.at(i) = false;
        }
        for(int i = 0; i < m_Mouse.size(); ++i) {
            m_Mouse.at(i) = false;
        }

        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: 
                    return true;
                    break;
                case SDL_KEYDOWN:
                    m_Keys.at(event.key.keysym.sym) = true;
                    break;
                case SDL_KEYUP:
                    m_Keys.at(event.key.keysym.sym) = false;
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    if(event.button.button == SDL_BUTTON_LEFT)
                        m_Mouse.at(MouseEvents::LeftButtonDown) = true;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        m_Mouse.at(MouseEvents::RightButtonDown) = true;
                    break;
                case SDL_MOUSEBUTTONUP: 
                    if(event.button.button == SDL_BUTTON_LEFT)
                        m_Mouse.at(MouseEvents::LeftButtonUp) = true;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        m_Mouse.at(MouseEvents::RightButtonUp) = true;
                    break;
                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0) 
                        m_Mouse.at(MouseEvents::WheelUp) = true;
                    else if(event.wheel.y < 0)
                        m_Mouse.at(MouseEvents::WheelDown) = true;
                    break;
            }
        }

        return false;

    }


    glm::ivec2 Application::GetMousePos() const {
        glm::ivec2 mouseCoords;
        SDL_GetMouseState(&mouseCoords.x, &mouseCoords.y);

        //move (0, 0) to center of screen and flip y-axis so that up is positive y
        mouseCoords.x -= GetWindowWidth() / 2;
        mouseCoords.y -= GetWindowHeight() /2;
        mouseCoords.y *= -1;

        //scale mousecoordinates from screen space to world space
        //mouse coords * (world space / screen space ratio)
        mouseCoords.x *= (GetProjWidth() / GetWindowWidth());
        mouseCoords.y *= (GetProjHeight() / GetWindowHeight());

        mouseCoords.x *= 1.0f / g_Scale;
        mouseCoords.y *= 1.0f / g_Scale;

        return mouseCoords;
    }




    void Application::Shutdown() const {
        m_Audio->Shutdown();
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


