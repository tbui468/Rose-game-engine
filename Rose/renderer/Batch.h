#ifndef BATCH_H
#define BATCH_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace rose {

    class Texture;
    class Shader;
    class VertexBuffer;
    class IndexBuffer;

    class Batch {
        public:
            Batch(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader);
            virtual ~Batch() {}
            void AddQuad();
            void BindAndUpload();

        private:
            std::vector<glm::mat4> m_Models;
            std::shared_ptr<Texture> m_Texture;
            std::shared_ptr<Shader> m_Shader;
            std::shared_ptr<VertexBuffer> m_VertexBuffer;
            std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };

}


#endif //BATCH_H
