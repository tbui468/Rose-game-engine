#include "renderer/IndexBuffer.h"

namespace rose {

    IndexBuffer::IndexBuffer() {
        glGenBuffers(1, &m_OpenGLID);
    }

    void IndexBuffer::AddIndex(const glm::ivec3& vec) {
        m_Indices.emplace_back(vec);
    }

    size_t IndexBuffer::Size() const {
        return m_Indices.size();
    }

    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGLID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_Indices.size() * 3, m_Indices.data(), GL_STATIC_DRAW);
    }


}
