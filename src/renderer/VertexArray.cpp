#include "renderer/VertexArray.h"

namespace rose {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_OpenGLID);
}
void VertexArray::Bind() {
    glBindVertexArray(m_OpenGLID);
}

}
