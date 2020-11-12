#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "glad/glad.h"

#include "Window.h"
#include "renderer/VertexArray.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"

namespace rose {

class Renderer {
    public:
        Renderer(std::shared_ptr<Window> window, bool vsync);
        virtual ~Renderer() {}
        void DrawQuad(); 
        void DrawScene(); 
    private:
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
};   

}

#endif //RENDERER_H
