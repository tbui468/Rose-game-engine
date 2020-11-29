#include "VertexBuffer.h"

namespace rose {

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &m_RendererID);
}
void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::SetData() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);
}

void VertexBuffer::AddVertex(const glm::vec3& position, const glm::vec2& texCoords) {
    Vertex vertex;
    vertex.Position = position;
    vertex.TexCoords = texCoords;
    m_Vertices.push_back(vertex);
}
void VertexBuffer::Clear() {
    m_Vertices.clear();
}

}
