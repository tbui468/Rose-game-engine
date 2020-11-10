#include "Squares.h"

extern const int32_t SCALE = 2;
extern const int32_t WINDOW_WIDTH = 480 * SCALE;
extern const int32_t WINDOW_HEIGHT = 270 * SCALE;


int main(int, char**) {

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



    sqs::CartCoords startCoords;
    startCoords.x = WINDOW_WIDTH * .5f;
    startCoords.y = WINDOW_HEIGHT * .5f - 50.0f;

    sqs::CartCoords quitCoords;
    quitCoords.x = WINDOW_WIDTH * .5f;
    quitCoords.y = WINDOW_HEIGHT * .5f + 50.0f;

    sqs::CartCoords closeCoords;
    closeCoords.x = WINDOW_WIDTH + 50;
    closeCoords.y = 0.0f + 50;

    std::shared_ptr<sqs::Entity> startButton = std::make_shared<sqs::Button>(startCoords, 0);
    std::shared_ptr<sqs::Entity> quitButton = std::make_shared<sqs::Button>(quitCoords, 0);
    std::shared_ptr<sqs::Entity> closeButton = std::make_shared<sqs::Button>(closeCoords, 0);

    startButton->SetBoundingBox(0.0f, 0.0f, 100, 50);
    quitButton->SetBoundingBox(0.0f, 0.0f, 100, 50);
    closeButton->SetBoundingBox(0.0f, 0.0f, 100, 50);

    SDL_Rect r;
    r.w = 100;
    r.h = 50;


    sqs::AnimationTimer timer;
    timer.SetSpeed(0.02f);

    std::vector<std::shared_ptr<sqs::Entity>> entityList;
    entityList.push_back(quitButton);
    entityList.push_back(startButton);
    entityList.push_back(closeButton);

    sqs::InputQueue inputQueue;

    bool quit = false;
    while(!quit) {

        inputQueue.PollEvents();

        sqs::CommandCode status = sqs::CommandCode::Failed;
        while(!inputQueue.Empty() && status == sqs::CommandCode::Failed) {
            sqs::CartCoordsi mouseCoords = inputQueue.GetMouseCoords();
            sqs::InputType input = inputQueue.NextInput();
            switch(input) {
                case sqs::InputType::Close: 
                    quit = true;
                    status = sqs::CommandCode::Success;
                    timer.ResetParameter();
                    break;
                case sqs::InputType::LeftTap: 
                    if(quitButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y)))
                        quit = true;
                    if(startButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {
                        startButton->MoveTo({startCoords.x + WINDOW_WIDTH, startCoords.y});
                        quitButton->MoveTo({quitCoords.x - WINDOW_WIDTH, quitCoords.y});
                        closeButton->MoveTo({WINDOW_WIDTH - 100, 50});
                        std::cout << "Clicked start button" << std::endl;
                    }
                    if(closeButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {
                        startButton->MoveTo(startCoords);
                        quitButton->MoveTo(quitCoords);
                        closeButton->MoveTo(closeCoords);
                        std::cout << "Clicked start button" << std::endl;
                    }
                    status = sqs::CommandCode::Success;
                    timer.ResetParameter();
                    break;
                case sqs::InputType::RightTap: 
                    status = sqs::CommandCode::Success;
                    timer.ResetParameter();
                    break;
                default:
                    status = sqs::CommandCode::Failed;
                    break;
            }
        }

        //std::cout << timer.GetParameter() << std::endl;

        timer.Update();
        float sigmoid = timer.GetSigmoidParameter();

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0xFD, 0xF6, 0xE3, 0x00);
        SDL_RenderClear(renderer);


        for(std::shared_ptr<sqs::Entity> b: entityList) {
            b->OnAnimationUpdate(sigmoid);
            if(timer.EndAnimation()) b->OnAnimationEnd();
            r.x = b->x - r.w * .5f;
            r.y = b->y - r.h * .5f;
            SDL_RenderDrawRect(renderer,&r);
            SDL_SetRenderDrawColor(renderer, 0x93, 0xA1, 0xA1, 0x00);
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

