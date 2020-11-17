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
        void AddEntity(const std::shared_ptr<Entity>& entity); //should just be called Draw
        uint32_t QuadCount() const { return m_Models.size(); }
        void ClearQuads();
        //BeginScene()
        void DrawScene(); 
        //EndScene() (clearQuads should be called here and it should also be private)
        void SetClearColor(const glm::ivec3& color);
        float GetProjWidth() const { return m_ProjWidth; }
        float GetProjHeight() const { return m_ProjHeight; }
    private:
        GLuint m_ShaderID;
        float m_ProjWidth {0.0f};
        float m_ProjHeight {0.0f};
        glm::mat4 m_Projection;
        std::vector<glm::mat4> m_Models;
        std::shared_ptr<Texture> m_Texture;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
};   

}

#endif //RENDERER_H
