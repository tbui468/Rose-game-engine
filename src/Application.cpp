#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Application.h"
#include "Button.h"
#include "AnimationTimer.h"
#include "CommandCode.h"
#include "InputQueue.h"
#include "Utility.h"

namespace rose {

    Application::Application() {
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "SDL_Init error!!!" << SDL_GetError() << std::endl;
        }

        bool fullScreen = true;
        m_Window = std::make_shared<Window>(960, 540, fullScreen);

        bool vsync = true;
        m_Renderer = std::make_shared<Renderer>(m_Window, vsync);

    }

    void Application::SetLayer(std::shared_ptr<Layer> layer) {
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

        //temp:
        AnimationTimer timer;
        timer.SetSpeed(0.001f);

        InputQueue inputQueue;

        /////////////OPENGL STUFF/////////////////////////////////
        ////////////////TEXTURES//////////////////////////////////
        int32_t width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        uint8_t *data = stbi_load("./../assets/Untitled.png", &width, &height, &channels, 0);

        if(stbi_failure_reason()) std::cout << stbi_failure_reason() << std::endl;
        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
        std::cout << "channels: " << channels << std::endl;



        //////////////SHADERS//////////////////////////
        float halfWidth = static_cast<float>(m_Window->GetWidth()) / 2.0f;
        float halfHeight = static_cast<float>(m_Window->GetHeight()) / 2.0f;
        glm::mat4 projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
        //view not necessary now since i have no camera
        //transformations for models
        glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f));

        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertexShaderCode = 
            "#version 330 core\n"
            "layout(location = 0) in vec3 vertexPos;"
            "layout(location = 1) in vec2 texCoords;"
            "uniform mat4 projection;"
            "uniform mat4 model;"
            "out vec2 v_texCoords;"
            "void main() {"
            "   gl_Position = projection * model * vec4(vertexPos, 1.0);"
            "   v_texCoords = texCoords;"
            "}";

        const char* fragmentShaderCode = 
            "#version 330 core\n"
            "in vec2 v_texCoords;"
            "out vec4 color;"
            "uniform sampler2D texSampler;"
            "void main() {"
            "   color = texture(texSampler, v_texCoords);"
            "}";

        GLint result = GL_FALSE;
        int infoLogLength;

        glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShaderID);

        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0) {
            std::vector<char> vertexShaderError(infoLogLength + 1);
            glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, vertexShaderError.data());
            printf("%s\n", vertexShaderError.data());
        }else{
            std::cout << "Vertex shader compiled!" << std::endl;
        }


        glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShaderID);

        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0) {
            std::vector<char> fragmentShaderError(infoLogLength + 1);
            glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, fragmentShaderError.data());
            printf("%s\n", fragmentShaderError.data());
        }else{
            std::cout << "Fragment shader compiled!" << std::endl;
        }

        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);
        //clean up (since we only need the compiled and linked program)
        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);


        GLuint textureID;
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glUniform1i(glGetUniformLocation(programID, "texSampler"), GL_TEXTURE0);


        while(!m_Quit) {
            m_Last = m_Now;
            m_Now = SDL_GetPerformanceCounter();
            m_DeltaTime = static_cast<double>((m_Now - m_Last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());

            m_Layer->Update();
            m_Layer->Draw();

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

            glUseProgram(programID);
            GLint projUniform = glGetUniformLocation(programID, "projection");
            glUniformMatrix4fv(projUniform, 1, GL_FALSE, (const float*)glm::value_ptr(projection));
            GLint modelUniform = glGetUniformLocation(programID, "model");
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, (const float*)glm::value_ptr(translation * rotation * scaling));
            m_Renderer->DrawScene();


            SDL_GL_SwapWindow(m_Window->GetHandle());

        }

        stbi_image_free(data);
    }

    void Application::Shutdown() {
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


