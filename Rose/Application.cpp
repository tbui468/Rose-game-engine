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


        char* exePath = GetExecutablePath();
        std::string exePathString(exePath);

        std::cout << exePathString << std::endl;

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
        result = system->createSound((exePathString + "../../assets/sound/pluck.wav").c_str(), 
                FMOD_DEFAULT, NULL, &sound);

        FMOD::Channel* channel;
        result = system->playSound(sound, NULL, false, &channel);
/*
        System::createSound(...) will return sound handle
        System::playSound(...) will return channel handle
        ChannelControl::isPlaying(...) to monitor channel, channel handle will be = FMOD_ERR_INVALID_HANDLE 
            when sound has ended
*/



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
            {
//                InstrumentationTimer timer("update");

                m_Last = m_Now;
                m_Now = SDL_GetPerformanceCounter();
                m_DeltaTime = static_cast<double>((m_Now - m_Last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());
                InputType input = GetInput();
                glm::ivec2 mousePos = GetMousePos();

                if(input == InputType::Quit) m_Quit = true;

                //if(input == InputType::LeftDown) play sound here
            
                m_Quit = m_Layer->Update(m_DeltaTime, input, mousePos);
            }
            {
 //               InstrumentationTimer timer("draw");
                m_Layer->Draw();
                m_Renderer->DrawScene();
                SDL_GL_SwapWindow(m_Window->GetHandle());
            }
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
                        return InputType::LeftDown;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        return InputType::RightDown;
                    break;
                case SDL_MOUSEBUTTONUP: 
                    if(event.button.button == SDL_BUTTON_LEFT)
                        return InputType::LeftUp;
                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        return InputType::RightUp;
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


