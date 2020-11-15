#include <SDL.h>

#include "Input.h"

namespace rose {

InputType Input::GetInput() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: return InputType::Close; break;
            case SDL_MOUSEBUTTONDOWN: 
                if(event.button.button == SDL_BUTTON_LEFT)
                    return InputType::LeftTap;
                else if(event.button.button == SDL_BUTTON_RIGHT)
                    return InputType::RightTap;
                break;
            default: return InputType::None; break;
        }
    }

    return InputType::None;
}


glm::ivec2 Input::GetMousePos() {
    glm::ivec2 mouseCoords;
    SDL_GetMouseState(&mouseCoords.x, &mouseCoords.y);
    return mouseCoords;
}



}
