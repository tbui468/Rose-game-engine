#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <vector>

namespace rose {


    class IndexBuffer {
        public:
            IndexBuffer() {
                glGenBuffers(1, &m_OpenGLID);
                //temp:
                m_Indices.push_back(0);
                m_Indices.push_back(1);
                m_Indices.push_back(2);
                m_Indices.push_back(0);
                m_Indices.push_back(2);
                m_Indices.push_back(3);
            }
            virtual ~IndexBuffer() {}
            void AddIndex(unsigned int index) {
                m_Indices.push_back(index);
            }
            void Bind() {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGLID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), m_Indices.data(), GL_STATIC_DRAW);
            }
        private:
            GLuint m_OpenGLID;
            std::vector<unsigned int> m_Indices;
    };


}

#endif //INDEX_BUFFER_H
