#ifndef UTILITY_H
#define UTILITY_H

#include <math.h>
#include <utility>
#include <SDL.h>

namespace sqs{


static int Init() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init error!!!" << SDL_GetError() << std::endl;
        return 1;
    }
    return 0;
}

template<typename T, typename... Args>
void CleanUp(T* t, Args&&...args) {
    CleanUp(t);
    CleanUp(std::forward<Args>(args)...);
}

template<>
inline void CleanUp<SDL_Window>(SDL_Window *win) {
    if(!win) return;
    SDL_DestroyWindow(win);
}
template<>
inline void CleanUp<SDL_Renderer>(SDL_Renderer *ren) {
    if(!ren) return;
    SDL_DestroyRenderer(ren);
}
template<>
inline void CleanUp<SDL_Texture>(SDL_Texture *tex) {
    if(!tex) return;
    SDL_DestroyTexture(tex);
}
template<>
inline void CleanUp<SDL_Surface>(SDL_Surface *surf) {
    if(!surf) return;
    SDL_FreeSurface(surf);
}


std::string GetAssetsPath(const std::string &subDir = "") {

#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif

    static std::string baseRes;
    if(baseRes.empty()) {
        char* basePath = SDL_GetBasePath();
        if(basePath) {
            baseRes = basePath;
            SDL_free(basePath);
        }else{
            std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
            return "";
        }
        size_t pos = baseRes.rfind("build");
        baseRes = baseRes.substr(0, pos) + "src" + PATH_SEP + "assets" + PATH_SEP;
    }

    return subDir.empty() ? baseRes: baseRes + subDir + PATH_SEP;

}

float Sigmoid(float _t) {
    return 1.0f / (1.0f + exp(-15.0f * (_t - 0.5f)));
}

}


#endif //UTILITY_H
