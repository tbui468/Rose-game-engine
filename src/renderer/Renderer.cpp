#include "renderer/Renderer.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace rose {

    Renderer::Renderer(std::shared_ptr<Window> window, bool vsync) {
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
        glClearColor(0.992f, 0.965f, 0.890f, 0.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        m_VertexArray = std::make_shared<VertexArray>();
        m_VertexArray->Bind();


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




        //////////////SHADERS//////////////////////////
        float halfWidth = static_cast<float>(window->GetWidth()) / 2.0f;
        float halfHeight = static_cast<float>(window->GetHeight()) / 2.0f;
        m_Projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);

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

        m_ShaderID = glCreateProgram();
        glAttachShader(m_ShaderID, vertexShaderID);
        glAttachShader(m_ShaderID, fragmentShaderID);
        glLinkProgram(m_ShaderID);
        //clean up (since we only need the compiled and linked program)
        glDetachShader(m_ShaderID, vertexShaderID);
        glDetachShader(m_ShaderID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);


        ////////////////TEXTURES//////////////////////////////////
        stbi_set_flip_vertically_on_load(true);

        //texture 0
        int32_t width, height, channels;
        uint8_t *data = stbi_load("./../assets/start.png", &width, &height, &channels, 0);

        if(stbi_failure_reason()) std::cout << stbi_failure_reason() << std::endl;

        GLuint textureID;
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glUniform1i(glGetUniformLocation(programID, "texSampler"), GL_TEXTURE0);

        stbi_image_free(data);

        int32_t width1, height1, channels1;
        uint8_t *data1 = stbi_load("./../assets/quit.png", &width1, &height1, &channels1, 0);

        if(stbi_failure_reason()) std::cout << stbi_failure_reason() << std::endl;

        GLuint textureID1;
        glGenTextures(1, &textureID1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, textureID1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glUniform1i(glGetUniformLocation(programID, "texSampler"), GL_TEXTURE1);

        stbi_image_free(data1);

        //texture 1

    }


    void Renderer::AddQuad(const glm::mat4& model) {
        m_Models.emplace_back(model);
    }

    void Renderer::ClearQuads() {
        m_Models.clear();
    }

    void Renderer::DrawScene() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //this is the vertex positions
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //this is the tex coords

        //set shader and set uniform for orthographic projection
        glUseProgram(m_ShaderID);
        GLint projUniform = glGetUniformLocation(m_ShaderID, "projection");
        glUniformMatrix4fv(projUniform, 1, GL_FALSE, (const float*)glm::value_ptr(m_Projection));

        for(glm::mat4& model: m_Models) {
//            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            GLint modelUniform = glGetUniformLocation(m_ShaderID, "model");
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, (const float*)glm::value_ptr(model));
            glUniform1i(glGetUniformLocation(m_ShaderID, "texSampler"), 1);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    }

}
