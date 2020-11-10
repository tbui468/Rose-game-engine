#include "Squares.h"


uint32_t SCALE = 2;
uint32_t WINDOW_WIDTH = 480 * SCALE;
uint32_t WINDOW_HEIGHT = 270 * SCALE;

int main(int, char**) {

    std::cout << sqs::GetAssetsPath() << std::endl;

    sqs::Init();

    SDL_Window* window = SDL_CreateWindow("Squares", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    } 

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if( renderer == nullptr) {
        sqs::CleanUp(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    //use stb_image to load png and create texture
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(texture == nullptr) {
        sqs::CleanUp(renderer, window);
        std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }



    sqs::CartCoords coords0;
    coords0.x = 0;
    coords0.y = 0;

    sqs::CartCoords coords1;
    coords1.x = 300;
    coords1.y = 200;

    std::shared_ptr<sqs::Button> button0 = std::make_shared<sqs::Button>(coords0, 0);
    std::shared_ptr<sqs::Button> button1 = std::make_shared<sqs::Button>(coords1, 0);

    button0->MoveTo(coords1);
    button1->MoveTo(coords0);

    SDL_Rect r;
    r.w = 100;
    r.h = 50;


    sqs::AnimationTimer timer;
    timer.SetSpeed(0.01f);

    std::vector<std::shared_ptr<sqs::Button>> buttonList;
    buttonList.push_back(button0);
    buttonList.push_back(button1);

    sqs::CommandDispatcher commandDispatcher;
    commandDispatcher.SetButtonList(&buttonList);

    sqs::InputQueue inputQueue;

    bool quit = false;
    while(!quit) {

        inputQueue.PollEvents();

        if(!inputQueue.Empty()) {
            sqs::InputType input = inputQueue.NextInput();
            sqs::CartCoordsi mouseCoords = inputQueue.GetMouseCoords();
            timer.ResetParameter();            
            sqs::CommandCode status = sqs::CommandCode::Failed;
            while(status == sqs::CommandCode::Failed) {
                status = commandDispatcher.Dispatch(input, mouseCoords.x, mouseCoords.y);
            }

            std::cout << mouseCoords.x << ", " << mouseCoords.y << std::endl;

            if(status == sqs::CommandCode::Quit) quit = true;
        }

        timer.Update();
        float sigmoid = timer.GetSigmoidParameter();



        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);


        for(std::shared_ptr<sqs::Button> b: buttonList) {
            b->OnAnimationUpdate(sigmoid);
            if(timer.EndAnimation()) b->OnAnimationEnd();
            r.x = b->x;
            r.y = b->y;
            SDL_RenderDrawRect(renderer,&r);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    sqs::CleanUp(texture, renderer, window);
    SDL_Quit();
    return 0;
}

