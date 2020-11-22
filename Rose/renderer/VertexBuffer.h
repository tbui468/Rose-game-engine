#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace rose {

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class VertexBuffer {
    public:
        VertexBuffer();
        virtual ~VertexBuffer() {}
        void Bind() const;
        void SetData();
        void AddVertex(const glm::vec3& position, const glm::vec2& texCoords);
        size_t GetVertexCount() const { return m_Vertices.size(); }
        void Clear();
    private:
        GLuint m_RendererID;
        std::vector<Vertex> m_Vertices;
};

}

#endif //VERTEX_BUFFER_H
