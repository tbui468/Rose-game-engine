#include "IndexBuffer.h"

namespace rose {

IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &m_RendererID);
}
void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::SetData() {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_Indices.size(), m_Indices.data(), GL_STATIC_DRAW);
}

void IndexBuffer::AddIndex(uint32_t index) {
    m_Indices.push_back(index);
}
void IndexBuffer::Clear() {
    m_Indices.clear();
}

}
