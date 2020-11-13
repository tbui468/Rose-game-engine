#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "renderer/Texture.h"

namespace rose {

class Renderer {
    public:
        Renderer(std::shared_ptr<Window> window, bool vsync);
        virtual ~Renderer() {}
        void AddQuad(const glm::mat4& model);
        void ClearQuads();
        void DrawScene(); 
    private:
        GLuint m_ShaderID;
        glm::mat4 m_Projection;
        std::vector<glm::mat4> m_Models;
        std::shared_ptr<Texture> m_Texture;
};   

}

#endif //RENDERER_H
