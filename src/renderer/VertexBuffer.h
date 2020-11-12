#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <glm/vec3.hpp>
#include <glad/glad.h>

namespace rose {

    class VertexBuffer {
        public:
            VertexBuffer();
            virtual ~VertexBuffer() {}
            void AddVertex(const glm::vec3& vec);
            void Bind() const;
        private:
            GLuint m_OpenGLID;
            std::vector<float> m_Vertices;
    };

}


#endif //VERTEX_BUFFER_H
