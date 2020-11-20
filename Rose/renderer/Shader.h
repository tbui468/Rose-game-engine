#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

namespace rose {


    class Shader {
        public:
            Shader(const char* vertex, const char* fragment);
            virtual ~Shader() {}
            void Bind() const;
            void SetUniformMatF(const char* name, int count, const float* data);
            void SetUniformI(const char* name, int data);

        private:
            GLuint m_RendererID;
            GLuint m_FragmentShaderID;
            GLuint m_VertexShaderID;
    };

}


#endif //SHADER_H
