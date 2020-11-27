#include <vector>

#include <SDL.h>

//fmod and COM
#include <combaseapi.h>
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>

//puttint FMOD headers before glad.h triggers APIENTRY redefinition warning
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "CommandCode.h"
#include "Instrumentor.h"

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

        //temp:
        //////////////////FMOD TEST//////////////////////
        //COM needs to be initialized and unizitialized for windows systems
        //not uninitializing COM is a memory leak
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        FMOD_RESULT result;
        FMOD::System* system = NULL;
        //create system object
        result = FMOD::System_Create(&system);
        if(result != FMOD_OK) {
            CoUninitialize();
            printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            exit(-1);
        }

        //init fmod
        result = system->init(512, FMOD_INIT_NORMAL, 0);
        if(result != FMOD_OK) {
            CoUninitialize();
            printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            exit(-1);
        }

        FMOD::Sound* sound;
        //FMOD_CREATESOUNDEXINFO exinfo;
        std::string exePathString(GetExecutablePath());
        result = system->createSound((exePathString + "../../assets/sound/pluck.wav").c_str(), 
                FMOD_DEFAULT, NULL, &sound);
/*
        FMOD::Channel* channel;
        result = system->playSound(sound, NULL, false, &channel);*/


        //ChannelControl::isPlaying(...) to monitor channel, channel handle will be = FMOD_ERR_INVALID_HANDLE

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
        for(bool b: m_Keys) b = false;
        for(bool b: m_Mouse) b = false;

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
                        m_Mouse.at(MouseEvents::LeftButton) = true;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        m_Mouse.at(MouseEvents::RightButton) = true;
                    break;
                case SDL_MOUSEBUTTONUP: 
                    if(event.button.button == SDL_BUTTON_LEFT)
                        m_Mouse.at(MouseEvents::LeftButton) = false;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        m_Mouse.at(MouseEvents::RightButton) = false;
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
        CoUninitialize();
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


