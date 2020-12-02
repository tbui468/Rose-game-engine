#ifndef UTILITY_H
#define UTILITY_H

#include <glm/glm.hpp>

namespace rose{


float PointDistance(const glm::ivec2& start, const glm::ivec2& end);


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
