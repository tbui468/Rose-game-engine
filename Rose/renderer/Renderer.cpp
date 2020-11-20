#include "renderer/Renderer.h"

#include "glm/gtx/string_cast.hpp"
#include "Globals.h"


namespace rose {

    Renderer::Renderer(std::shared_ptr<Window> window, bool vsync, const std::string& exePath) {
        SDL_GLContext glContext;
        glContext = SDL_GL_CreateContext(window->GetHandle());
        if(glContext == NULL)
            std::cout << "Could not create OpenGl context" << std::endl;
        else
            std::cout << "OpenGL context created!" << std::endl;

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        // Also request a depth buffer
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        gladLoadGLLoader(SDL_GL_GetProcAddress);
        SDL_GL_SetSwapInterval(vsync);
        int w,h;
        SDL_GetWindowSize(window->GetHandle(), &w, &h);
        glViewport(0, 0, w, h);
        SetClearColor(glm::ivec3(0, 0, 0));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLuint vertexArray;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);



        //Shader stuff
        const char* vertexShaderCode = 
            "#version 330 core\n"
            "layout(location = 0) in vec3 vertexPos;"
            "layout(location = 1) in vec2 texCoords;"
            "layout(location = 2) in int modelIndex;"
            "uniform mat4 projection;"
            "uniform mat4 models[128];"
            "out vec2 v_texCoords;"
            "void main() {"
            "   gl_Position = projection * models[modelIndex] * vec4(vertexPos, 1.0);"
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


        m_Shader = std::make_shared<Shader>(vertexShaderCode, fragmentShaderCode);



        m_Texture = std::make_shared<Texture>(0);
        m_Texture->LoadTexture(exePath + "../../assets/textureSheet.png");
        m_Texture->AddSprite("StartButton", { glm::ivec2(0, 96), glm::ivec2(64, 32)});
        m_Texture->AddSprite("QuitButton", { glm::ivec2(64, 0), glm::ivec2(64, 32)});
        m_Texture->AddSprite("CloseButton", { glm::ivec2(96, 96), glm::ivec2(32, 32)});


        m_ProjWidth = 480.0f * g_Scale;
        m_ProjHeight = 270.0f * g_Scale;
        m_Projection = glm::ortho(-m_ProjWidth / 2, m_ProjWidth / 2, -m_ProjHeight / 2, m_ProjHeight / 2, -1.0f, 1.0f);

        m_BatchDefaultTex = std::make_shared<Batch>(m_Texture, m_Shader, m_Projection);
        //m_BatchCustomTex = std::make_shared<Batch>(m_Texture, m_Shader, m_Projection);
    }


    void Renderer::AddEntity(const Entity* entity) {
        m_BatchDefaultTex->AddQuad(entity);
    }



    void Renderer::SetClearColor(const glm::ivec3& color) {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 0.0f);
    }

    void Renderer::DrawScene() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //this is the vertex positions
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))); //this is the tex coords
        glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void*)(5 * sizeof(float))); //this is the tex coords
/*
        m_Shader->Bind();
        m_Shader->SetUniformMatF("projection", 1, (const float*)glm::value_ptr(m_Projection));
        m_Shader->SetUniformMatF("models", m_Models.size(), (const float*)glm::value_ptr(m_Models.at(0)));
        m_Shader->SetUniformI("texSampler", 0);

        m_VertexBuffer->Bind();
        m_VertexBuffer->SetData();
        m_IndexBuffer->Bind();
        m_IndexBuffer->SetData();

        glDrawElements(GL_TRIANGLES, m_IndexBuffer->Count(), GL_UNSIGNED_INT, (void*)0); //this should draw from index buffer (and corresponding vertex buffer)*/
        
        m_BatchDefaultTex->Bind();
        glDrawElements(GL_TRIANGLES, m_BatchDefaultTex->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
        m_BatchDefaultTex->ClearBuffers();
        /*
        m_BatchCustomTex->Bind();
        glDrawElements(GL_TRIANGLES, m_BatchCustomTex->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
        m_BatchCustomTex->ClearBuffers();*/

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

    }

}
