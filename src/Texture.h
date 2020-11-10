#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>

namespace sqs {

class Texture {
    public:
        Texture(Renderer* renderer, Window* window) {
            m_Handle = SDL_CreateTexture(renderer->GetHandle(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window->GetWidth(), window->GetHeight());
            if(m_Handle == nullptr) {
                std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
                SDL_DestroyRenderer(renderer->GetHandle());
                SDL_DestroyWindow(window->GetHandle());
                SDL_Quit();
            }
        }
        virtual ~Texture() {}
        SDL_Texture* GetHandle() const { return m_Handle; }
    private:
        SDL_Texture* m_Handle;
};

}


#endif //TEXTURE_H
