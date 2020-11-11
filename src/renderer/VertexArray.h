#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "glad/glad.h"

namespace rose {

    class VertexArray {
        public:
            VertexArray() {
                glGenVertexArrays(1, &m_OpenGLID);
            }
            void Bind() {
                glBindVertexArray(m_OpenGLID);
            }
            virtual ~VertexArray() {}
        private:
            GLuint m_OpenGLID;
    };

}


#endif
