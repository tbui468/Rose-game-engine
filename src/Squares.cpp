#include "Squares.h"


uint32_t SCALE = 2;
uint32_t WINDOW_WIDTH = 480 * SCALE;
uint32_t WINDOW_HEIGHT = 270 * SCALE;

//button class (including animated class)
//event system to get mouse/keyboard input

int main(int, char**) {

    sqs::Init();

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

    bool quit = false;

    SDL_Rect r;
    r.w = 100;
    r.h = 50;

    sqs::CartCoords coords0;
    coords0.x = 0;
    coords0.y = 0;

    sqs::CartCoords coords1;
    coords1.x = 300;
    coords1.y = 200;

    sqs::Button button0(coords0, 0);
    sqs::Button button1(coords1, 0);

    button0.MoveTo(coords1);
    button1.MoveTo(coords0);

    SDL_Rect r0;
    SDL_Rect r1;


    sqs::AnimationTimer timer;
    timer.SetSpeed(0.0075f);

    sqs::EntityList<Button> buttonList;
    buttonList.Add(button0);
    buttonList.Add(button1);

    sqs::InputQueue inputQueue;

    while(!quit) {

        inputQueue.PollEvents();

        if(!inputQueue.Empty()) {
            sqs::InputType input = inputQueue.NextInput();
            switch(input) {
                case sqs::InputType::Close: quit = true; break;
            }
          //  commandDispatcher.Dispatch(input);
        }

        timer.Update();
        float sigmoid = timer.GetSigmoidParameter();

        for(Button& b: buttonList) {
            b.OnAnimationUpdate(sigmoid);
            if(timer.EndAnimation()) b.OnAnimationEnd();
        }

/*
        button0.OnAnimationUpdate(sigmoid);
        button1.OnAnimationUpdate(sigmoid);

        if(timer.EndAnimation()) {
            button0.OnAnimationEnd();
            button1.OnAnimationEnd();
        }*/


        r0.x = button0.x;
        r0.y = button0.y;
        r0.w = 100;
        r0.h = 50;


        r1.x = button1.x;
        r1.y = button1.y;
        r1.w = 100;
        r1.h = 50;

        //draw
        //renderer.Clear(color);
        //renderer.SetShader();
        //renderer.Draw(button0);
        //renderer.Draw(fractals);
        //renderer.

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_RenderDrawRect(renderer,&r0);
        SDL_RenderDrawRect(renderer,&r1);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r0);
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r1);
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

