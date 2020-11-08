#include <iostream>

#include <SDL.h>

int main(int, char**) {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL init error!!!" << std::endl;
        return 1;
    }
    std::cout << "SDL init okay!" << std::endl;
    SDL_Quit();
    return 0;
}
