#include <SDL.h>

#include "Input.h"
#include "Application.h"

namespace rose {

InputType Input::GetInput() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: 
                return InputType::Quit; 
                break;
            case SDL_MOUSEBUTTONDOWN: 
                if(event.button.button == SDL_BUTTON_LEFT)
                    return InputType::LeftTap;
                else if(event.button.button == SDL_BUTTON_RIGHT)
                    return InputType::RightTap;
                break;
            case SDL_MOUSEBUTTONUP: 
                if(event.button.button == SDL_BUTTON_LEFT)
                    return InputType::LeftRelease;
                else if(event.button.button == SDL_BUTTON_RIGHT)
                    return InputType::RightRelease;
                break;
            default: 
                return InputType::None; 
                break;
        }
    }

    return InputType::None;
}


glm::ivec2 Input::GetMousePos() {
    glm::ivec2 mouseCoords;
    SDL_GetMouseState(&mouseCoords.x, &mouseCoords.y);

    Application* app = Application::GetApplication();
    int32_t windowWidth = app->GetWindowWidth();
    int32_t windowHeight = app->GetWindowHeight();

    //move (0, 0) to center of screen and flip y-axis so that up is positive y
    mouseCoords.x -= windowWidth / 2;
    mouseCoords.y -= windowHeight /2;
    mouseCoords.y *= -1;

    //scale mousecoordinates from screen space to world space
    //mouse coords * (world space / screen space ratio)
    float projWidth = app->GetProjWidth();
    float projHeight = app->GetProjHeight();
    mouseCoords.x *= (projWidth / windowWidth);
    mouseCoords.y *= (projHeight / windowHeight);

    return mouseCoords;
}



}
