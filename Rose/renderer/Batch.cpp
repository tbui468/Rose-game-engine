#include "renderer/Batch.h"
#include "renderer/Texture.h"
#include "renderer/Shader.h"

namespace rose {

Batch::Batch(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader) {
    m_Texture = texture;
    m_Shader = shader;
}


}
