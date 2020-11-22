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
            "uniform mat4 projection;"
            "out vec2 v_texCoords;"
            "void main() {"
            "   gl_Position = projection * vec4(vertexPos, 1.0);"
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
        m_Shader1 = std::make_shared<Shader>(vertexShaderCode, fragmentShaderCode);



        m_TextureDefault = std::make_shared<Texture>(exePath + "../../assets/textureSheet.png");

        m_TextureCustom = std::make_shared<Texture>(256, 256); //temp dimension to test copying default texture directly



        m_ProjWidth = 480.0f * g_Scale;
        m_ProjHeight = 270.0f * g_Scale;
        m_Projection = glm::ortho(-m_ProjWidth / 2, m_ProjWidth / 2, -m_ProjHeight / 2, m_ProjHeight / 2, -1.0f, 1.0f);


        m_BatchCustomTex = std::make_shared<Batch>(m_TextureCustom, m_Shader1, m_Projection);
        m_BatchDefaultTex = std::make_shared<Batch>(m_TextureDefault, m_Shader, m_Projection);

        //checking stuff
        int maxUniformVectors;
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxUniformVectors);
        std::cout << "Max uniform vectors: " << maxUniformVectors << std::endl;
    }


    void Renderer::AddEntity(const Entity* entity) {
        const Sprite& sprite = entity->GetSprite();

        switch(sprite.TexType) {
            case TextureType::None: 
                std::cout << "Texture type not specifided" << std::endl; 
                assert(false); 
                break;
            case TextureType::Default:
                m_BatchDefaultTex->AddQuad(entity);
                break;
            case TextureType::Custom: 
                m_BatchCustomTex->AddQuad(entity);
                break;
            default:
                std::cout << "Texture type not specified" << std::endl;
                assert(false);
                break;
        }
        
    }



    void Renderer::SetClearColor(const glm::ivec3& color) {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 0.0f);
    }

    void Renderer::DrawScene() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        if(m_BatchCustomTex->GetIndexCount() > 0) {
            m_BatchCustomTex->Bind();
            glDrawElements(GL_TRIANGLES, m_BatchCustomTex->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            m_BatchCustomTex->ClearBuffers();
        }


        if(m_BatchDefaultTex->GetIndexCount() > 0) {
            m_BatchDefaultTex->Bind();
            glDrawElements(GL_TRIANGLES, m_BatchDefaultTex->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
            m_BatchDefaultTex->ClearBuffers();
        }

        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        


    }


    void Renderer::SetCustomTexture(const std::vector<SubTextureMapping>& subtextures) {
        for(const SubTextureMapping& texMapping : subtextures) {
            m_TextureCustom->CopySubTexture(texMapping.DesTexCoords, m_TextureDefault->GetID(), texMapping.SrcTexCoords, texMapping.TexDimensions.x, texMapping.TexDimensions.y);
        }
    }

}
