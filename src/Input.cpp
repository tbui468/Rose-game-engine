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

    //transform so center of screen has coordinates (0, 0)
    //and flip y axis so up is positive y
    Application* app = Application::GetApplication();
    int32_t windowWidth = app->GetWindowWidth();
    int32_t windowHeight = app->GetWindowHeight();

    mouseCoords.x -= windowWidth / 2;
    mouseCoords.y -= windowHeight /2;
    mouseCoords.y *= -1;

    return mouseCoords;
}



}
