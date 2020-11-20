#ifndef BATCH_H
#define BATCH_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "renderer/IndexBuffer.h"

namespace rose {

    class Texture;
    class Shader;
    class VertexBuffer;
    class Entity;

    class Batch {
        public:
            Batch(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, const glm::mat4& proj);
            virtual ~Batch() {}
            void AddQuad(const Entity* entity);
            void ClearBuffers();
            void Bind();
            size_t GetIndexCount() const { return m_IndexBuffer->Count(); }
            int QuadCount() const { return m_Models.size(); }

        private:
            glm::mat4 m_Projection;
            std::shared_ptr<Texture> m_Texture;
            std::shared_ptr<Shader> m_Shader;
            std::vector<glm::mat4> m_Models;
            std::shared_ptr<VertexBuffer> m_VertexBuffer;
            std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };

}


#endif //BATCH_H
