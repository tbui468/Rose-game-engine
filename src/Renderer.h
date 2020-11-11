#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "glad/glad.h"

namespace rse {

class Renderer {
    public:
        Renderer(Window* window, bool vsync) {
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
            glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
        }
        virtual ~Renderer() {}
    private:
};   

}

#endif //RENDERER_H
