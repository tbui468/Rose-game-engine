#include "renderer/Shader.h"
#include <cstdio>

namespace rose {

    Shader::Shader(const char* vertexShaderCode, const char* fragmentShaderCode) {
        //for error checking
        GLint result = GL_FALSE;
        int infoLogLength;

        m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        //vertex shader compilation/check
        glShaderSource(m_VertexShaderID, 1, &vertexShaderCode, NULL);
        glCompileShader(m_VertexShaderID);
/*
        glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(m_VertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0) {
            std::vector<char> vertexShaderError(infoLogLength + 1);
            glGetShaderInfoLog(m_VertexShaderID, infoLogLength, NULL, vertexShaderError.data());
            printf("%s\n", vertexShaderError.data());
        }else{
            std::cout << "Vertex shader compiled!" << std::endl;
        }*/

        //fragment shader compilation/check
        glShaderSource(m_FragmentShaderID, 1, &fragmentShaderCode, NULL);
        glCompileShader(m_FragmentShaderID);
/*
        glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(m_FragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0) {
            std::vector<char> fragmentShaderError(infoLogLength + 1);
            glGetShaderInfoLog(m_FragmentShaderID, infoLogLength, NULL, fragmentShaderError.data());
            printf("%s\n", fragmentShaderError.data());
        }else{
            std::cout << "Fragment shader compiled!" << std::endl;
        }*/


        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, m_VertexShaderID);
        glAttachShader(m_RendererID, m_FragmentShaderID);
        glLinkProgram(m_RendererID);
        //clean up (since we only need the compiled and linked program)
        glDetachShader(m_RendererID, m_VertexShaderID);
        glDetachShader(m_RendererID, m_FragmentShaderID);
        glDeleteShader(m_VertexShaderID);
        glDeleteShader(m_FragmentShaderID);
    }
    void Shader::Bind() const {
        glUseProgram(m_RendererID);
    }


    void Shader::SetUniformMatF(const char* name, int count, const float* data) {
        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name), count, GL_FALSE, data);
    }
    void Shader::SetUniformI(const char* name, int data) {
        glUniform1i(glGetUniformLocation(m_RendererID, name), data);
    }

}


