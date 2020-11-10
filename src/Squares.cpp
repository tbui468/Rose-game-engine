#include "Squares.h"

#include "glad/glad.h"


int main(int, char**) {



    sqs::Init();
    bool fullScreen = false;
    sqs::Window* window = new sqs::Window(960, 540, fullScreen);

    bool vsync = false;
    sqs::Renderer* renderer = new sqs::Renderer(window, vsync);

    sqs::Texture* texture = new sqs::Texture(renderer, window);

    /////////////////OpenGL test////////////////
    SDL_GLContext glContext;
    glContext = SDL_GL_CreateContext(window->GetHandle());
    if(glContext == NULL)
        std::cout << "Could not create OpenGl context" << std::endl;
    else
        std::cout << "OpenGL context created!" << std::endl;

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
    int w,h;
    SDL_GetWindowSize(window->GetHandle(), &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    static const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /////////////////////////////////////////////////////////

    float windowWidth = static_cast<float>(window->GetWidth());
    float windowHeight = static_cast<float>(window->GetHeight());

    sqs::CartCoords startCoords;
    startCoords.x = windowWidth * .5f;
    startCoords.y = windowHeight * .5f - 50.0f;

    sqs::CartCoords quitCoords;
    quitCoords.x = windowWidth * .5f;
    quitCoords.y = windowHeight * .5f + 50.0f;

    sqs::CartCoords closeCoords;
    closeCoords.x = windowWidth + 100.0f;
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
    timer.SetSpeed(0.001f);

    std::vector<std::shared_ptr<sqs::Entity>> entityList;
    entityList.push_back(quitButton);
    entityList.push_back(startButton);
    entityList.push_back(closeButton);

    sqs::InputQueue inputQueue;

    //delta time
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t last = 0;
    double deltaTime = 0.0;


    bool quit = false;
    while(!quit) {
        last = now;
        now = SDL_GetPerformanceCounter();
        deltaTime = static_cast<double>((now - last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());

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
                    /*
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
                    }*/
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

        timer.Update(deltaTime);
        float sigmoid = timer.GetSigmoidParameter();

        //opengl stuff
        glClear(GL_COLOR_BUFFER_BIT);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        SDL_GL_SwapWindow(window->GetHandle());
/*

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
        SDL_RenderPresent(renderer->GetHandle());*/
    }

    sqs::CleanUp(texture, renderer, window);
    sqs::Shutdown();

    return 0;
}

