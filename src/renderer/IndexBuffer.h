#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace rose {

class IndexBuffer {
    public:
        IndexBuffer();
        virtual ~IndexBuffer() {}
        void Bind() const;
        void SetData();
        void AddIndex(uint32_t index);
        size_t Count() const { return m_Indices.size(); }
        void Clear();
    private:
        GLuint m_RendererID;
        std::vector<uint32_t> m_Indices;
};


}

#endif //INDEX_BUFFER_H
