#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "CommandCode.h"

namespace rose {


    Application* Application::GetApplication() {
        if(!s_Application) {
            s_Application = new Application();
        }

        return s_Application;
    }

    std::shared_ptr<Renderer> Application::GetRenderer() const {
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

            InputType input = GetInput();
            glm::ivec2 mousePos = GetMousePos();

            if(input == InputType::Quit) m_Quit = true;


            m_Renderer->ClearQuads();
            m_Quit = m_Layer->Update(m_DeltaTime, input, mousePos);
            m_Layer->Draw();


            m_Renderer->DrawScene();

            SDL_GL_SwapWindow(m_Window->GetHandle());
        }

    }


    InputType Application::GetInput() const {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: 
                    return InputType::Quit; 
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    if(event.button.button == SDL_BUTTON_LEFT)
                        return InputType::LeftTap;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        return InputType::RightTap;
                    break;
                case SDL_MOUSEBUTTONUP: 
                    if(event.button.button == SDL_BUTTON_LEFT)
                        return InputType::LeftRelease;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        return InputType::RightRelease;
                    break;
                default: 
                    return InputType::None; 
                    break;
            }
        }

        return InputType::None;
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

        return mouseCoords;
    }




    void Application::Shutdown() const {
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


