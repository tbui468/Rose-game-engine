#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>

namespace rose {

    class VertexBuffer {
        public:
            VertexBuffer() {
                glGenBuffers(1, &m_OpenGLID);
                //temp:
                m_Vertices.push_back(-0.5f);
                m_Vertices.push_back(-0.5f);
                m_Vertices.push_back(0.0f);

                m_Vertices.push_back(0.5f);
                m_Vertices.push_back(-0.5f);
                m_Vertices.push_back(0.0f);

                m_Vertices.push_back(0.5f);
                m_Vertices.push_back(0.5f);
                m_Vertices.push_back(0.0f);

                m_Vertices.push_back(-0.5f);
                m_Vertices.push_back(0.5f);
                m_Vertices.push_back(0.0f);
            }
            virtual ~VertexBuffer() {}
            void AddVertex() {
            }
            void Bind() {
                glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);
            }
        private:
            GLuint m_OpenGLID;
            std::vector<float> m_Vertices;
    };

}


#endif //VERTEX_BUFFER_H
