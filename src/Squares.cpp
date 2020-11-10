#include "Squares.h"


int main(int, char**) {

    sqs::Init();

    sqs::Window* window = new sqs::Window(960, 540, true);

    sqs::Renderer* renderer = new sqs::Renderer(window);

    sqs::Texture* texture = new sqs::Texture(renderer, window);


    float windowWidth = static_cast<float>(window->GetWidth());
    float windowHeight = static_cast<float>(window->GetHeight());

    sqs::CartCoords startCoords;
    startCoords.x = windowWidth * .5f;
    startCoords.y = windowHeight * .5f - 50.0f;

    sqs::CartCoords quitCoords;
    quitCoords.x = windowWidth * .5f;
    quitCoords.y = windowHeight * .5f + 50.0f;

    sqs::CartCoords closeCoords;
    closeCoords.x = windowWidth + 50.0f;
    closeCoords.y = 0.0f + 50.f;

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
                    std::cout << mouseCoords.x << ", " << mouseCoords.y << std::endl;
                    if(quitButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y)))
                        quit = true;
                    if(startButton->PointCollision(static_cast<float>(mouseCoords.x), static_cast<float>(mouseCoords.y))) {
                        startButton->MoveTo({startCoords.x + windowWidth, startCoords.y});
                        quitButton->MoveTo({quitCoords.x - windowWidth, quitCoords.y});
                        closeButton->MoveTo({windowWidth - 100.0f, 50.f});
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

        timer.Update();
        float sigmoid = timer.GetSigmoidParameter();

        SDL_SetRenderTarget(renderer->GetHandle(), texture->GetHandle());
        SDL_SetRenderDrawColor(renderer->GetHandle(), 0xFD, 0xF6, 0xE3, 0x00);
        SDL_RenderClear(renderer->GetHandle());


        for(std::shared_ptr<sqs::Entity> b: entityList) {
            b->OnAnimationUpdate(sigmoid);
            if(timer.EndAnimation()) b->OnAnimationEnd();
            r.x = b->x - r.w * .5f;
            r.y = b->y - r.h * .5f;
            SDL_RenderDrawRect(renderer->GetHandle(),&r);
            SDL_SetRenderDrawColor(renderer->GetHandle(), 0x93, 0xA1, 0xA1, 0x00);
            SDL_RenderFillRect(renderer->GetHandle(), &r);
        }

        SDL_SetRenderTarget(renderer->GetHandle(), NULL);
        SDL_RenderCopy(renderer->GetHandle(), texture->GetHandle(), NULL, NULL);
        SDL_RenderPresent(renderer->GetHandle());
    }

    sqs::CleanUp(texture, renderer, window);
    sqs::Shutdown();

    return 0;
}

