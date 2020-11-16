#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Entity.h"
#include "renderer/Texture.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"

namespace rose {

class Renderer {
    public:
        Renderer(std::shared_ptr<Window> window, bool vsync);
        virtual ~Renderer() {}
        void AddEntity(std::shared_ptr<Entity> entity);
        uint32_t QuadCount() const { return m_Models.size(); }
        void ClearQuads();
        void DrawScene(); 
        void SetClearColor(const glm::ivec3& color);
    private:
        GLuint m_ShaderID;
        glm::mat4 m_Projection;
        std::vector<glm::mat4> m_Models;
        std::shared_ptr<Texture> m_Texture;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
};   

}

#endif //RENDERER_H
