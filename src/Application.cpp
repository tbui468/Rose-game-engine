#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "Button.h"
#include "AnimationTimer.h"
#include "CommandCode.h"
#include "InputQueue.h"
#include "Input.h"
//#include "Utility.h"

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

            m_Layer->Update();
            m_Layer->Draw();

         //   inputQueue.PollEvents();
/*
            CommandCode status = CommandCode::Failed;
            while(!inputQueue.Empty() && status == CommandCode::Failed) {
                glm::ivec2 mouseCoords = inputQueue.GetMouseCoords();
                InputType input = inputQueue.NextInput();
                switch(input) {
                    case InputType::Close: 
                        m_Quit = true;
                        status = CommandCode::Success;
                        timer.ResetParameter();
                        break;
                        
                    case InputType::LeftTap: 

                        std::cout << mouseCoords.x << ", " << mouseCoords.y << std::endl;
                        if(quitButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y)))
                            m_Quit = true;
                        if(startButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {

                            startButton->MoveTo({startCoords.x + windowWidth * .6f, startCoords.y});
                            quitButton->MoveTo({quitCoords.x - windowWidth * .6f, quitCoords.y});
                            startButton->ScaleTo({4.0f, 1.0f});
                            quitButton->ScaleTo({2.0f, 4.0f});
                            closeButton->MoveTo({800.0f, 0.0f});
                            std::cout << "Clicked start button" << std::endl;
                        }
                        if(closeButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {
                            startButton->MoveTo(startCoords);
                            quitButton->MoveTo(quitCoords);
                            startButton->ScaleTo({2.0f, 1.0f});
                            quitButton->ScaleTo({2.0f, 1.0f});
                            closeButton->MoveTo(closeCoords);
                            std::cout << "Clicked start close" << std::endl;
                        }
                        status = CommandCode::Success;
                        timer.ResetParameter();
                        break;
                    case InputType::RightTap: 
                        status = CommandCode::Success;
                        timer.ResetParameter();
                        break;
                    default:
                        status = CommandCode::Failed;
                        break;
                }
            }*/


            timer.Update(m_DeltaTime);
            float sigmoid = timer.GetSigmoidParameter();

            startButton->OnAnimationUpdate(sigmoid);
            quitButton->OnAnimationUpdate(sigmoid);
            closeButton->OnAnimationUpdate(sigmoid);

            if(timer.EndAnimation()) {
                startButton->OnAnimationEnd();
                quitButton->OnAnimationEnd();
                closeButton->OnAnimationEnd();
            }

            m_Renderer->ClearQuads();

            m_Renderer->AddEntity(startButton);
            m_Renderer->AddEntity(quitButton);
            m_Renderer->AddEntity(closeButton);


            m_Renderer->DrawScene();

            SDL_GL_SwapWindow(m_Window->GetHandle());
        }

    }

    void Application::Shutdown() {
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


