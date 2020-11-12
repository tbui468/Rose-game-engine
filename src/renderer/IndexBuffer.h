#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <vector>
#include <glad/glad.h>
#include <glm/vec3.hpp>

namespace rose {


    class IndexBuffer {
        public:
            IndexBuffer();
            virtual ~IndexBuffer() {}
            void AddIndex(const glm::ivec3& vec);
            void Bind() const;
            size_t Size() const;
        private:
            GLuint m_OpenGLID;
            std::vector<glm::ivec3> m_Indices;
    };


}

#endif //INDEX_BUFFER_H
