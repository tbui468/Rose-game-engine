#include <vector>

#include <SDL.h>
#include "glad/glad.h"

#include "Application.h"
#include "Button.h"
#include "AnimationTimer.h"
#include "CommandCode.h"
#include "InputQueue.h"
#include "Utility.h"

namespace rse {

    void Application::Init() {
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "SDL_Init error!!!" << SDL_GetError() << std::endl;
        }

        bool fullScreen = false;
        m_Window = new Window(960, 540, fullScreen);

        bool vsync = true;
        m_Renderer = new Renderer(m_Window, vsync);



    }


    void Application::SetLayer(Layer* layer) {
        m_Layer = layer;
    }

    //should update and draw layers in here
    void Application::Run() {

        float windowWidth = static_cast<float>(m_Window->GetWidth());
        float windowHeight = static_cast<float>(m_Window->GetHeight());

        CartCoords startCoords;
        startCoords.x = windowWidth * .5f;
        startCoords.y = windowHeight * .5f - 50.0f;

        CartCoords quitCoords;
        quitCoords.x = windowWidth * .5f;
        quitCoords.y = windowHeight * .5f + 50.0f;

        CartCoords closeCoords;
        closeCoords.x = windowWidth + 100.0f;
        closeCoords.y = 0.0f + 50.f;

        std::shared_ptr<Entity> startButton = std::make_shared<Button>(startCoords, 0);
        std::shared_ptr<Entity> quitButton = std::make_shared<Button>(quitCoords, 0);
        std::shared_ptr<Entity> closeButton = std::make_shared<Button>(closeCoords, 0);

        startButton->SetBoundingBox(0.0f, 0.0f, 100, 50);
        quitButton->SetBoundingBox(0.0f, 0.0f, 100, 50);
        closeButton->SetBoundingBox(0.0f, 0.0f, 100, 50);



        std::vector<std::shared_ptr<Entity>> entityList;
        entityList.push_back(quitButton);
        entityList.push_back(startButton);
        entityList.push_back(closeButton);



        //delta time
        m_Now = SDL_GetPerformanceCounter();
        m_Last = 0;
        m_DeltaTime = 0.0;




        /////////////////OpenGL: setting up data test////////////////
        //temp: just to test open gl

        GLuint vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        static const GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        /////////////////////////////////////////////////////////
        //temp:
        AnimationTimer timer;
        timer.SetSpeed(0.001f);

        InputQueue inputQueue;


        while(!m_Quit) {
            m_Last = m_Now;
            m_Now = SDL_GetPerformanceCounter();
            m_DeltaTime = static_cast<double>((m_Now - m_Last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());

            inputQueue.PollEvents();

            CommandCode status = CommandCode::Failed;
            while(!inputQueue.Empty() && status == CommandCode::Failed) {
                CartCoordsi mouseCoords = inputQueue.GetMouseCoords();
                InputType input = inputQueue.NextInput();
                switch(input) {
                    case InputType::Close: 
                        m_Quit = true;
                        status = CommandCode::Success;
                        timer.ResetParameter();
                        break;
                    case InputType::LeftTap: 
                        /*
                           std::cout << mouseCoords.x << ", " << mouseCoords.y << std::endl;
                           if(quitButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y)))
                           quit = true;
                           if(startButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {
                           startButton->MoveTo({startCoords.x + windowWidth, startCoords.y});
                           quitButton->MoveTo({quitCoords.x - windowWidth, quitCoords.y});
                           closeButton->MoveTo({windowWidth - 100.0f, 50.f});
                           std::cout << "Clicked start button" << std::endl;
                           }
                           if(closeButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {
                           startButton->MoveTo(startCoords);
                           quitButton->MoveTo(quitCoords);
                           closeButton->MoveTo(closeCoords);
                           std::cout << "Clicked start button" << std::endl;
                           }*/
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
            }

            timer.Update(m_DeltaTime);
            float sigmoid = timer.GetSigmoidParameter();

            //opengl stuff
            glClear(GL_COLOR_BUFFER_BIT);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glDisableVertexAttribArray(0);
            SDL_GL_SwapWindow(m_Window->GetHandle());


        }
    }

    void Application::Shutdown() {
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


