#ifndef UTILITY_H
#define UTILITY_H

#include <utility>
#include <memory>
#include <SDL.h>

namespace rose{


struct CartCoords {
    float x = 0.0f;
    float y = 0.0f;
};

using Scale = CartCoords;

struct CartCoordsi {
    int x = 0;
    int y = 0;
};


struct PolarCoords {
    float r = 0.0f;
    float a = 0.0f;
};

struct Color {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

struct Rectangle {
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;
};

/*
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

}*/


}


#endif //UTILITY_H
