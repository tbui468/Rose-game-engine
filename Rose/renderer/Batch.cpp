#include "renderer/Batch.h"
#include "renderer/Texture.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "Entity.h"

namespace rose {

Batch::Batch(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Shader>& shader, const glm::mat4& proj) {
    m_Texture = texture;
    m_Shader = shader;
    m_Projection = proj;

    m_VertexBuffer = std::make_shared<VertexBuffer>();

    m_IndexBuffer = std::make_shared<IndexBuffer>();
}

void Batch::AddQuad(const Entity* entity) {
    float texWidth = static_cast<float>(m_Texture->GetWidth());
    float texHeight = static_cast<float>(m_Texture->GetHeight());

    glm::mat4 model = entity->GetModelMatrix();
    const Sprite& sprite = entity->GetSprite();
    glm::vec2 texCoordsStart = { sprite.TexCoords.x / texWidth, sprite.TexCoords.y / texHeight };
    glm::vec2 texCoordsEnd = {texCoordsStart.x + sprite.TexDimensions.x / texWidth, texCoordsStart.y + sprite.TexDimensions.y / texHeight};

    m_VertexBuffer->AddVertex(glm::vec3(model * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f)), texCoordsStart);
    m_VertexBuffer->AddVertex(glm::vec3(model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f)), {texCoordsEnd.x, texCoordsStart.y});
    m_VertexBuffer->AddVertex(glm::vec3(model * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f)), texCoordsEnd);
    m_VertexBuffer->AddVertex(glm::vec3(model * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)), {texCoordsStart.x, texCoordsEnd.y});

    size_t indexPos = 4 * QuadCount();
    m_IndexBuffer->AddIndex(indexPos);
    m_IndexBuffer->AddIndex(indexPos + 1);
    m_IndexBuffer->AddIndex(indexPos + 2);
    m_IndexBuffer->AddIndex(indexPos);
    m_IndexBuffer->AddIndex(indexPos + 2);
    m_IndexBuffer->AddIndex(indexPos + 3);
    m_Models.push_back(model); 
}

void Batch::ClearBuffers() {
    m_Models.clear();
    m_VertexBuffer->Clear();
    m_IndexBuffer->Clear();
}

void Batch::Bind() {
    m_Texture->Bind();    

    m_Shader->Bind();
    m_Shader->SetUniformMatF("projection", 1, (const float*)glm::value_ptr(m_Projection));
    m_Shader->SetUniformI("texSampler", GL_TEXTURE0);

    m_VertexBuffer->Bind();
    m_VertexBuffer->SetData();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //this is the vertex positions
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))); //this is the tex coords

    m_IndexBuffer->Bind();
    m_IndexBuffer->SetData();
}


}
