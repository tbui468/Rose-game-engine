#include "renderer/VertexBuffer.h"

namespace rose {

    VertexBuffer::VertexBuffer() {
        glGenBuffers(1, &m_OpenGLID);
    }

    void VertexBuffer::AddVertex(const glm::vec3& vec) {
        m_Vertices.emplace_back(vec.x);
        m_Vertices.emplace_back(vec.y);
        m_Vertices.emplace_back(vec.z);
    }

    void VertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);
    }


}
