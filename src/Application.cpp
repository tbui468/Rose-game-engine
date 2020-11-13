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
        startCoords.x = 0.0f;
        startCoords.y = -100.0f;

        CartCoords quitCoords;
        quitCoords.x = 0.0f;
        quitCoords.y = 100.0f;

        CartCoords closeCoords;
        closeCoords.x = 1200.0f;
        closeCoords.y = 0.0f;

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
        timer.SetSpeed(0.0005f);

        InputQueue inputQueue;

        /////////////OPENGL STUFF/////////////////////////////////
        //temp: testing AddVertex and AddIndex functions
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        const float vertices[]= {
            -32.0f, -32.0f, 0.0f, 0.0f, 0.0f,
            32.0f, -32.0f, 0.0f, 1.0f, 0.0f,
            32.0f, 32.0f, 0.0f, 1.0f, 1.0f,
            -32.0f, 32.0f, 0.0f, 0.0f, 1.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 5, &vertices[0], GL_STATIC_DRAW);

        GLuint indexBuffer;
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        const uint32_t indices[]={
            0, 1, 2, 0, 2, 3
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, &indices[0], GL_STATIC_DRAW);

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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
                mouseCoords.x -= windowWidth / 2.0f;
                mouseCoords.y -= windowHeight / 2.0f;
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
                            quitButton->ScaleTo({1.0f, 4.0f});
                            closeButton->MoveTo({800.0f, 0.0f});
                            std::cout << "Clicked start button" << std::endl;
                        }
                        if(closeButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {
                            startButton->MoveTo(startCoords);
                            quitButton->MoveTo(quitCoords);
                            startButton->ScaleTo({1.0f, 1.0f});
                            quitButton->ScaleTo({1.0f, 1.0f});
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
            }

            //start button
            glm::mat4 scaling0 = glm::scale(glm::mat4(1.0f), glm::vec3(startButton->xScale, startButton->yScale, 1.0f));
            glm::mat4 rotation0 = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translation0 = glm::translate(glm::mat4(1.0f), glm::vec3(startButton->x, startButton->y, 0.0f));

            //quit button
            glm::mat4 scaling1 = glm::scale(glm::mat4(1.0f), glm::vec3(quitButton->xScale, quitButton->yScale, 1.0f));
            glm::mat4 rotation1 = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translation1 = glm::translate(glm::mat4(1.0f), glm::vec3(quitButton->x, quitButton->y, 0.0f));

            //close button
            glm::mat4 scaling2 = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 rotation2 = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translation2 = glm::translate(glm::mat4(1.0f), glm::vec3(closeButton->x, closeButton->y, 0.0f));


            //update entites here (position and such)
            //apply these transformations to the transformations matrices

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

            glUseProgram(programID);
            GLint projUniform = glGetUniformLocation(programID, "projection");
            glUniformMatrix4fv(projUniform, 1, GL_FALSE, (const float*)glm::value_ptr(projection));
            //m_Renderer->DrawScene();
            //
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //this is the vertex positions
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //this is the tex coords

            GLint modelUniform = glGetUniformLocation(programID, "model");
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, (const float*)glm::value_ptr(translation0 * rotation0 * scaling0));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

            modelUniform = glGetUniformLocation(programID, "model");
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, (const float*)glm::value_ptr(translation1 * rotation1 * scaling1));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

            modelUniform = glGetUniformLocation(programID, "model");
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, (const float*)glm::value_ptr(translation2 * rotation2 * scaling2));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);


            glDisableVertexAttribArray(0);


            SDL_GL_SwapWindow(m_Window->GetHandle());

        }

        stbi_image_free(data);
    }

    void Application::Shutdown() {
        SDL_DestroyWindow(m_Window->GetHandle());
        SDL_Quit();
    }

}


