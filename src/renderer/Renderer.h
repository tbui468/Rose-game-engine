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
        void AddQuad(); //calling this only adds more vertices and indices, then all of them are drawn at once at the end
        void DrawScene(); //use all the vertex/index data to draw all quads (at the end of the loop)
    private:
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
};   

}

#endif //RENDERER_H
