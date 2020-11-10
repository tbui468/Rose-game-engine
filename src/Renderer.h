#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

namespace sqs {

class Renderer {
    public:
        Renderer(Window* window, bool vsync) {
            if(vsync) m_Handle = SDL_CreateRenderer(window->GetHandle(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            else m_Handle = SDL_CreateRenderer(window->GetHandle(), -1, SDL_RENDERER_ACCELERATED);

            if( m_Handle == nullptr) {
                std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(window->GetHandle());
                SDL_Quit();
            }
        }
        virtual ~Renderer() {}
        SDL_Renderer* GetHandle() const { return m_Handle; }
    private:
        SDL_Renderer* m_Handle;
};   

}

#endif //RENDERER_H
