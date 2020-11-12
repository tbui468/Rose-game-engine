#include "renderer/Renderer.h"

#include <glm/vec3.hpp>

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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        m_VertexArray = std::make_shared<VertexArray>();
        m_VertexArray->Bind();

        //temp: testing AddVertex and AddIndex functions
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        const float vertices[]= {
            -100.0f, -100.0f, 0.0f, 0.0f, 0.0f,
            100.0f, -100.0f, 0.0f, 1.0f, 0.0f,
            100.0f, 100.0f, 0.0f, 1.0f, 1.0f,
            -100.0f, 100.0f, 0.0f, 0.0f, 1.0f
        };
/*        const float vertices[]= {
            -100.0f, -100.0f, 0.0f,
            100.0f, -100.0f, 0.0f,
            100.0f, 100.0f, 0.0f,
            -100.0f, 100.0f, 0.0f
        };*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 5, &vertices[0], GL_STATIC_DRAW);

        GLuint indexBuffer;
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        const uint32_t indices[]={
            0, 1, 2, 0, 2, 3
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, &indices[0], GL_STATIC_DRAW);
        /*
        m_VertexBuffer = std::make_shared<VertexBuffer>();

        m_VertexBuffer->AddVertex(glm::vec3(-100.0f, -100.0f, 0.0f));
        m_VertexBuffer->AddVertex(glm::vec3(100.0f, -100.0f, 0.0f));
        m_VertexBuffer->AddVertex(glm::vec3(100.0f, 100.0f, 0.0f));
        m_VertexBuffer->AddVertex(glm::vec3(-100.0f, 100.0f, 0.0f));

        m_VertexBuffer->AddVertex(glm::vec3(100.0f, 100.0f, 0.0f));
        m_VertexBuffer->AddVertex(glm::vec3(300.0f, 100.0f, 0.0f));
        m_VertexBuffer->AddVertex(glm::vec3(300.0f, 300.0f, 0.0f));
        m_VertexBuffer->AddVertex(glm::vec3(100.0f, 300.0f, 0.0f));

        m_VertexBuffer->Bind();*/
/*
        m_IndexBuffer = std::make_shared<IndexBuffer>();
        m_IndexBuffer->AddIndex(glm::ivec3(0, 1, 2));
        m_IndexBuffer->AddIndex(glm::ivec3(0, 2, 3));
        //m_IndexBuffer->AddIndex(glm::ivec3(4, 5, 6));
        //m_IndexBuffer->AddIndex(glm::ivec3(4, 6, 7));
        m_IndexBuffer->Bind();*/
    }


    void Renderer::DrawQuad() {
        //this should really just be a shader (transformations and texture)
        //add this to a list of other Quads to draw
    }

    void Renderer::DrawScene() {
        //get list of quads to draw
        //should set shader once (shouldn't really need more than 1??)
        //then set uniforms for the current quad in list
        //draw quad
        //repeat until all quads are drawn
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
//        m_VertexBuffer->Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //this is the vertex positions
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //this is the tex coords
        //m_IndexBuffer->Bind();
        //glDrawElements(GL_TRIANGLES, 3 * m_IndexBuffer->Size(), GL_UNSIGNED_INT, (void*)0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
    }

}
