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

        /////////////OPENGL STUFF/////////////////////////////////
        m_VertexBuffer = std::make_shared<VertexBuffer>();
        m_VertexBuffer->Bind();

        m_IndexBuffer = std::make_shared<IndexBuffer>();
        m_IndexBuffer->Bind();


        //////////////SHADERS//////////////////////////
        m_ProjWidth = 480.0f * g_Scale;
        m_ProjHeight = 270.0f * g_Scale;
        m_Projection = glm::ortho(-m_ProjWidth / 2, m_ProjWidth / 2, -m_ProjHeight / 2, m_ProjHeight / 2, -1.0f, 1.0f);

        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertexShaderCode = 
            "#version 330 core\n"
            "layout(location = 0) in vec3 vertexPos;"
            "layout(location = 1) in vec2 texCoords;"
            "layout(location = 2) in int modelIndex;"
            "uniform mat4 projection;"
            "uniform mat4 models[64];"
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

        m_ShaderID = glCreateProgram();
        glAttachShader(m_ShaderID, vertexShaderID);
        glAttachShader(m_ShaderID, fragmentShaderID);
        glLinkProgram(m_ShaderID);
        //clean up (since we only need the compiled and linked program)
        glDetachShader(m_ShaderID, vertexShaderID);
        glDetachShader(m_ShaderID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);


        m_Texture = std::make_shared<Texture>();
        //m_Texture->LoadTexture("./../assets/textureSheet.png");
        m_Texture->LoadTexture(exePath + "../../assets/textureSheet.png");
        m_Texture->AddSprite("StartButton", { glm::ivec2(0, 96), glm::ivec2(64, 32)});
        m_Texture->AddSprite("QuitButton", { glm::ivec2(64, 0), glm::ivec2(64, 32)});
        m_Texture->AddSprite("CloseButton", { glm::ivec2(96, 96), glm::ivec2(32, 32)});

    }


    void Renderer::AddEntity(Entity* entity) {
        float texWidth = static_cast<float>(m_Texture->GetWidth());
        float texHeight = static_cast<float>(m_Texture->GetHeight());

        glm::mat4 model = entity->GetModelMatrix();
        const Sprite& sprite = entity->GetSprite();
        glm::vec2 texCoordsStart = { sprite.TexCoords.x / texWidth, sprite.TexCoords.y / texHeight };
        glm::vec2 texCoordsEnd = {texCoordsStart.x + sprite.TexDimensions.x / texWidth, texCoordsStart.y + sprite.TexDimensions.y / texHeight};

        m_VertexBuffer->AddVertex(glm::vec3(-0.5f, -0.5f, 0.0f), texCoordsStart, QuadCount());
        m_VertexBuffer->AddVertex(glm::vec3(0.5f, -0.5f, 0.0f), {texCoordsEnd.x, texCoordsStart.y}, QuadCount());
        m_VertexBuffer->AddVertex(glm::vec3(0.5f, 0.5f, 0.0f), texCoordsEnd, QuadCount());
        m_VertexBuffer->AddVertex(glm::vec3(-0.5f, 0.5f, 0.0f), {texCoordsStart.x, texCoordsEnd.y}, QuadCount());

        size_t indexPos = 4 * QuadCount();
        m_IndexBuffer->AddIndex(indexPos);
        m_IndexBuffer->AddIndex(indexPos + 1);
        m_IndexBuffer->AddIndex(indexPos + 2);
        m_IndexBuffer->AddIndex(indexPos);
        m_IndexBuffer->AddIndex(indexPos + 2);
        m_IndexBuffer->AddIndex(indexPos + 3);
        m_Models.push_back(model); 
    }

    void Renderer::ClearQuads() {
        m_Models.clear();
        m_VertexBuffer->Clear();
        m_IndexBuffer->Clear();
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

        glUseProgram(m_ShaderID);
        //projection
        GLint projUniform = glGetUniformLocation(m_ShaderID, "projection");
        glUniformMatrix4fv(projUniform, 1, GL_FALSE, (const float*)glm::value_ptr(m_Projection));
        //model array
        GLint modelUniform = glGetUniformLocation(m_ShaderID, "models");
        glUniformMatrix4fv(modelUniform, QuadCount(), GL_FALSE, (const float*)glm::value_ptr(m_Models.at(0)));
        //texture
        glUniform1i(glGetUniformLocation(m_ShaderID, "texSampler"), 0);

        m_VertexBuffer->Bind();
        m_VertexBuffer->SetData();
        m_IndexBuffer->Bind();
        m_IndexBuffer->SetData();

        glDrawElements(GL_TRIANGLES, m_IndexBuffer->Count(), GL_UNSIGNED_INT, (void*)0); //this should draw from index buffer (and corresponding vertex buffer)

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

    }

}
