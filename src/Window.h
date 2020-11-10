#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

namespace sqs {

class Window {
    public:
        Window(int32_t width, int32_t height, bool fullscreen): m_Width(width), m_Height(height) {
            m_Handle = SDL_CreateWindow("Squares", 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
            if(m_Handle == nullptr) {
                std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
                SDL_Quit();
            } 

            if(fullscreen) {
                SDL_SetWindowFullscreen(m_Handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
                SDL_DisplayMode dm;
                SDL_GetDesktopDisplayMode(0, &dm);
                m_Width = dm.w;
                m_Height = dm.h;
            }
        }
        virtual ~Window() {}
        int32_t GetWidth() const { return m_Width; }
        int32_t GetHeight() const { return m_Height; }
        SDL_Window* GetHandle() const { return m_Handle; }
    private:
        SDL_Window* m_Handle;
        int32_t m_Width;
        int32_t m_Height;
};

}


#endif //WINDOW_H
