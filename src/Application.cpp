#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "Button.h"
#include "AnimationTimer.h"
#include "CommandCode.h"
#include "Input.h"
#include "Logger.h"

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

        bool fullScreen = false;
        m_Window = std::make_shared<Window>(960, 540, fullScreen);

        bool vsync = true;
        m_Renderer = std::make_shared<Renderer>(m_Window, vsync);



    }

    void Application::SetLayer(std::shared_ptr<Layer> layer) {
        m_Layer = layer;
    }


    void Application::SetClearColor(const glm::ivec3& color) {
        m_Renderer->SetClearColor(color);
    }

    void Application::Run() {

        //all code up until while() loop is client code (eg, put it inside Squares.cpp)
        //then wire up all the references needed for Layer Update() and Draw() functions to take care of everything

        float windowWidth = static_cast<float>(m_Window->GetWidth());
        float windowHeight = static_cast<float>(m_Window->GetHeight());


        glm::vec2 startCoords;
        startCoords.x = 0.0f;
        startCoords.y = 100.0f;

        glm::vec2 quitCoords;
        quitCoords.x = 0.0f;
        quitCoords.y = -100.0f;

        glm::vec2 closeCoords;
        closeCoords.x = 1200.0f;
        closeCoords.y = 0.0f;

        std::shared_ptr<Entity> startButton = std::make_shared<Button>("StartButton", startCoords);
        std::shared_ptr<Entity> quitButton = std::make_shared<Button>("QuitButton", quitCoords);
        std::shared_ptr<Entity> closeButton = std::make_shared<Button>("CloseButton", closeCoords);


        //delta time
        m_Now = SDL_GetPerformanceCounter();
        m_Last = 0;
        m_DeltaTime = 0.0;

        //temp:
        AnimationTimer timer;
        timer.SetSpeed(0.001f);

        //InputQueue inputQueue(m_Window);



        while(!m_Quit) {
            m_Last = m_Now;
            m_Now = SDL_GetPerformanceCounter();
            m_DeltaTime = static_cast<double>((m_Now - m_Last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());

            m_Renderer->ClearQuads();
            m_Layer->Update();
            m_Layer->Draw();

            timer.Update(m_DeltaTime);
            float sigmoid = timer.GetSigmoidParameter();
/*
            startButton->OnAnimationUpdate(sigmoid);
            quitButton->OnAnimationUpdate(sigmoid);
            closeButton->OnAnimationUpdate(sigmoid);

            if(timer.EndAnimation()) {
                startButton->OnAnimationEnd();
                quitButton->OnAnimationEnd();
                closeButton->OnAnimationEnd();
            }

*/
 //           m_Renderer->AddEntity(startButton);
  //          m_Renderer->AddEntity(quitButton);
   //         m_Renderer->AddEntity(closeButton);
            //Draw(startButton);

            m_Renderer->DrawScene();

            SDL_GL_SwapWindow(m_Window->GetHandle());
        }

    }


//    void Application::Draw(const std::string& spriteName, const glm::mat4& model) {

 //   }

    void Application::Draw(const std::shared_ptr<Entity>& entity) {
        m_Renderer->AddEntity(entity);
    }

    void Application::Shutdown() {
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


