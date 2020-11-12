#include "renderer/Renderer.h"

namespace rose {

    Renderer::Renderer(std::shared_ptr<Window> window, bool vsync) {
        SDL_GLContext glContext;
        glContext = SDL_GL_CreateContext(window->GetHandle());
        if(glContext == NULL)
            std::cout << "Could not create OpenGl context" << std::endl;
        else
            std::cout << "OpenGL context created!" << std::endl;

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        // Also request a depth buffer
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        gladLoadGLLoader(SDL_GL_GetProcAddress);
        SDL_GL_SetSwapInterval(vsync);
        int w,h;
        SDL_GetWindowSize(window->GetHandle(), &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.992f, 0.965f, 0.890f, 0.0f);


        m_VertexArray = std::make_shared<VertexArray>();
        m_VertexArray->Bind();
        m_VertexBuffer = std::make_shared<VertexBuffer>();
        m_VertexBuffer->Bind();
        m_IndexBuffer = std::make_shared<IndexBuffer>();
        m_IndexBuffer->Bind();
    }


    void Renderer::AddQuad() {
        //add group of 4 vertices
        //add the corresponding indices (6 for a quad)
    }

    void Renderer::DrawScene() {
        glClear(GL_COLOR_BUFFER_BIT);
        glEnableVertexAttribArray(0);
        m_VertexBuffer->Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        m_IndexBuffer->Bind();
        glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
    }

}
