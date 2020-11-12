#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "glad/glad.h"

namespace rose {

    class VertexArray {
        public:
            VertexArray();
            virtual ~VertexArray() {}
            void Bind();
        private:
            GLuint m_OpenGLID;
    };

}


#endif
