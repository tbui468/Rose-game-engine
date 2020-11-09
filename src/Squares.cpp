#include <SDL.h>

#include "Squares.h"


uint32_t SCALE = 2;
uint32_t WINDOW_WIDTH = 480 * SCALE;
uint32_t WINDOW_HEIGHT = 270 * SCALE;

//button class (including animated class)
//event system to get mouse/keyboard input

int main(int, char**) {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init error!!!" << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Squares", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    } 

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if( renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    //use stb_image to load png and create texture
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(texture == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool quit = false;

    SDL_Rect r;
    r.w = 100;
    r.h = 50;


    while(!quit) {
        //events
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) quit = true;
        }

        //draw
        r.x=rand()%500;
        r.y=rand()%500;

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer,&r);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }


    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

